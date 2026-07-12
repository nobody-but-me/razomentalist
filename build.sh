
#!/usr/bin/env bash

# my basic build bash script

echo -e "[config]: Are you generating ninja files? (y or n)(DEFAULT: n)"
read -p " > " build
echo -e "[config]: Do you want to run the project right after building it? (y or n)(DEFAULT: y)"
read -p " > " run

echo "[info]: creating build folder..."

mkdir -p ./build && cd ./build

if [ "$build" = "y" ]; then
    echo -e "[info]: generating ninja files..."
	cmake .. -G Ninja -DCMAKE_C_COMPILER=gcc
fi

echo -e "[info]: building..."
ninja

cd ..

if  [ "$run" != "n" ]; then
    echo -e "\n--------------------------------------------------\n"
    ./run.sh
fi

