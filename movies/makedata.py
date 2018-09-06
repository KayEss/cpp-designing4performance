#!/usr/bin/env python3
from json import loads
import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT, ISOLATION_LEVEL_SERIALIZABLE
import sys


if __name__ == "__main__":

    try:
        psycopg2.connect(database='pacpp')
        print("Managed to connect... best delete")
        with psycopg2.connect('') as cnx:
            cnx.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)
            cnx.cursor().execute("DROP DATABASE pacpp")
    except:
        pass
    print("Create database 'pacpp'")
    cnx = psycopg2.connect('')
    cnx.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)
    cnx.cursor().execute("CREATE DATABASE pacpp")

    cnx = psycopg2.connect('dbname=pacpp')
    cnx.set_isolation_level(ISOLATION_LEVEL_SERIALIZABLE)
    cursor = cnx.cursor()

    cursor.execute('''
        CREATE TABLE films (
            id int NOT NULL,
            CONSTRAINT smalltable_pk PRIMARY KEY (id),
            name_en text NOT NULL,
            popularity real
        );
    ''')
    cnx.commit()

    with open("movie_ids_07_18_2018.json", "rt") as movies:
        count = 0
        while movies:
            count += 1
            moviestr = movies.readline()
            if not moviestr:
                cnx.commit()
                print(count, " rows inserted")
                sys.exit(0)
            movie = loads(moviestr)
            values = (movie["id"], movie["original_title"], movie["popularity"])
            cursor.execute('''INSERT INTO films VALUES (%s, %s, %s)''', values)
            if count % 20 == 0:
                print(*values)
                cnx.commit()


