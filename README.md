
# TODD-BOT

Todd is an open-source truth or dare bot for discord, written in C++ using the [D++](https://github.com/brainboxdotcc/DPP) library


![Screenshot](https://i.ibb.co/KwjgYpz/IloveCpp.png)


## Building

To build the project first make sure you have [DPP](https://dpp.dev) installed.

```bash
  g++ -std=c++17 -o bot src/main.cpp src/question-parser.cpp src/database.cpp -ldpp -lsqlite3 
```
## Installation

Install dependencies

```bash
  wget -O dpp.deb https://dl.dpp.dev/
  sudo dpkg -i dpp.deb
```
#### and then just run the bot

### If you get an error regarding libssl.so.3:

```bash
  ln -s libssl.so.3 libssl.so
  sudo ldconfig
```

#### from [Stack Overflow](https://stackoverflow.com/questions/54124906/openssl-error-while-loading-shared-libraries-libssl-so-3)    
