Link for the solution: https://github.com/TryGhost/node-sqlite3/issues/933

Steps:

1. Explained: Basically, installing Node-SQLite3 in Raspberry Pi might need to build it from source. Also, the library might depend on libsqlite3-dev.
2. Under your project where it contains package.json, steps: 

        sudo apt-get install libsqlite3-dev

        npm install sqlite3 --build-from-source --sqlite=/usr