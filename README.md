To start app in terminal need

> cd C:\Users\user\coursework1.2.0
> cmake --build build

But to build a new build

> cd C:\Users\user\coursework1.2.0
> rmdir /s /q build
> cmake -S . -B build -G "MinGW Makefiles"
> cmake --build build
