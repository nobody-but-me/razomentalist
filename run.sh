
#!/usr/bin/env bash

if [ -f ./build/bin/razomentalist ]; then
    echo -e "[info]: running...\n"
    ./build/bin/razomentalist
else
	echo -e "[error]: could not run application: executable does not exist or is corrupted.\n"
fi
