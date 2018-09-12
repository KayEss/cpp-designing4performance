#include "json.hpp"

#include <pgasio/connection.hpp>
#include <pgasio/buffered.hpp>
#include <pgasio/query.hpp>


std::string &safe_data(std::string &str, pgasio::byte_view text) {
    return str.append(reinterpret_cast<const char *>(text.data()), text.size());
}
std::string &safe_string(std::string &str, pgasio::byte_view text) {
    return safe_data(str += '"', text) += '"';
}

void stringify(pgasio::byte_view txt, pgasio::column_meta def, std::string &into) {
    if ( txt.data() == nullptr ) {
        into += "null";
    } else {
        switch ( def.field_type_oid ) {
        case 16: // bool
            into += txt[0] == 't' ? "true" : "false";
            break;
        case 21: // int2
        case 23: // int4
        case 20: // int8
        case 26: // oid
        case 700: // float4
        case 701: // float8
            safe_data(into, txt);
            break;
        case 114: // json
        case 1700: // numeric
        case 1082: // date
        case 1083: // time
        case 1114: // timestamp without time zone
        case 1184: // timestamp with time zone
        case 2950: // uuid
        case 3802: // jsonb
            safe_string(into, txt);
            break;
        default:
            into += "**** " +
                std::to_string(def.field_type_oid) + " **** ";
        case 25: // text
        case 1042: // bpchar
        case 1043: // varchar
            escape(f5::u8view{reinterpret_cast<const char *>(txt.data()), txt.size()}, into);
        }
    }
}


int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Pass an SQL statement to execute" << std::endl;
        std::exit(1);
    }

    /// The parameters we need to use.
    const char *user = std::getenv("LOGNAME");
    const char *database = std::getenv("PGDATABASE");
    const char *path = "/var/run/postgresql/.s.PGSQL.5432";
    const char *sql = argv[1];

    boost::asio::io_service ios;
    boost::asio::spawn(ios, [&](auto yield) {
        auto cnx = pgasio::handshake(
            pgasio::make_buffered(pgasio::unix_domain_socket(ios, path, yield)),
            user, database, yield);
        auto results = pgasio::query(cnx, sql, yield);
        auto recordset = results.recordset(yield);

        std::string buffer;
        buffer.reserve(640 * 1024);
        for ( auto col : recordset.columns() ) {
            if ( not buffer.empty() ) buffer += ',';
            escape(f5::u8view{col.name}, buffer);
        }
        for ( auto block{recordset.next_block(yield)}; block; block = recordset.next_block(yield) ) {
            auto fields = block.fields();
            while ( fields.size() ) {
                auto row{fields.slice(0, recordset.columns().size())};
                bool first{true};
                for ( std::size_t index{}; index < recordset.columns().size(); ++index ) {
                    if ( index ) buffer += ',';
                    stringify(row[index], recordset.columns()[index], buffer);
                }
                fields = fields.slice(recordset.columns().size());
                buffer += '\n';
                if ( buffer.size() > 600 * 1024 ) {
                    std::cout << buffer;
                    buffer.clear();
                }
            }
        }
        std::cout << buffer;
    });
    ios.run();

    return 0;
}

