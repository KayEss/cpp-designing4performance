# Designing APIs For Performance

This repository contains the code, scripts and slides for the talk at Pacific++ 2018.

The actual code that was run is in the `real` folder, and the code for the slides is in the `slides` folder.


## Building

Normal cmake rules apply:

```bash
git clone --recursive git@github.com:KayEss/pacificpp18.git
mkdir pacificpp18/build.tmp; cd pacificpp18/build.tmp
ccmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja
```


## The data

First you should get Postgres configured so that ident authentication works. You can check if this is the case or not by just typing `psql` and seeing if you can access the server without any further options.

On an Ubuntu machine something like the following should work (a superuser role):

```bash
sudo -u postgres createuser -ls $LOGNAME
sudo -u postgres createdb -o $LOGNAME $LOGNAME
psql
```

The command lines can be found in the slides. They assume the build directory is the current working directory.


### Movies

The film list is from the [daily list export](https://developers.themoviedb.org/3/getting-started/daily-file-exports) from [The Movie DB](https://www.themoviedb.org/). You'll need to fix up the file name in [./movies/makedata.py]. You'll also need to install the Python packages outlined in [./movies/setup.pip]. Running `pip install -r setup.pip` from that directory should suffice (assumes Python 3 is the default).


### `pgbench`

Use pgbench to create the table and data for the `pgbench_accounts`. For the talk the following was used:

```bash
pgbench -i -s 20
```
