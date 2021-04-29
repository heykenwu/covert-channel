<pre>
------------------------------
Title   : COMP 8505 FINAL PROJECT
Author  : Eric Wu, Hong-Kit Wu
Date    : 2020-12-06
Version : 5
------------------------------

----------------------  Introduction & Background ---------------------------
-----------------------------------------------------------------------------
Design and implement a complete covert application that will allow a user to 
access a port (that is otherwise closed) o a firewall, communicate with a
"disguised" backdoor application. The backdoor application will accept
commands and execute them; the results of the command execution will be sent
back to the remote client application

-----------------------------  Basic Usage  ---------------------------------
-----------------------------------------------------------------------------
Use below commands to run the executable (each one on seperate linux machine)
Example:    ./handler.elf
            ./backdoor.elf
To issue command a to backdoor, simply type in a command like you normall would
do with a linux terminal. To exfiltrate a file from the backdoor, issue the
command "exfilt-file" follow by the name of the file you want to get. Full path
name must be used. For instance, to get the the shadown file, type the 
following (dont type the "[CMD]:", this is just to show what it looks like on
the screen)

[CMD]: exfilt-file /etc/shadow

The backdoor installs a keylogger when run. However, to receive the file
containing the recorded keystrokes, the user must use the "exfilt-file" function
to do so. The backdoor does not send back the keystroke file automatically.
The saved keystroke file location is the same as what is specifed in the
"setting.json" config file (for additional details, refer "How to setup &
prerequisite" section)
-----------------------------------------------------------------------------

--------------------------  Project Structure  ------------------------------
-----------------------------------------------------------------------------
├── backdoor
│   ├── build           # C object files\
│   ├── src             # C Source files and header files\
│   |   └── header
│   |   └── ...
│   └── Makefile
|
├── handler
│   ├── build           # C object files
│   ├── src             # C Source files and header files
│   |   └── header
│   |   └── ...
│   └── Makefile
|
├── keylogger
│   ├── build           # C object files
│   ├── src             # C Source files and header files
│   |   └── header
│   |   └── ...
│   └── Makefile
|
├── setting.json         # Settings read by setup.sh
├── setup.sh             # Automated build scripts
└── README.md

---------------------  How to setup & prerequisite  -------------------------
-----------------------------------------------------------------------------
Make sure the JSON parser 'jq' is install. The module can be install by simply
run 'apt-get install jq'. In addition, a webserver is required to host the
keylogger executables. Python proivdes an easy to use webserver module called
"http.server".
-----------------------------------------------------------------------------

----------------------
| Step 1\
----------------------
-> Fill in the require filed in setting.json
(a template named setting_template.json is provided)
Command fields
* NIC_NAME          - name of network iterface used
* IP                - ip address
* CMD_CHANNEL_PORT  - port used for port knocking, send/receive command
* FILE_EXFILT_PORT  - port used for exfiltrating files back backdoor 
* HEADER_KEY        - a string value, used for authentication
* PASSWORD          - a string value, used for encryption
* mask_name         - a string value, name used for disguising the process 

Handler specific fields
* exfilt_dir        - location to store exfiltrated files

backdoor specific fileds
* watch_list        - list of files to watch for creation. Whenever a file
                      in the watch list is created, it will be sent back to
                      the handler.
    * keylogger
        name        - name of the keylogger file
        url         - url of the webserver hosting the keylogger
        location    - keylogger install location
        save_at     - keystrokes file save location

----------------------
| Step 2
----------------------
-> Compile and generate executables
cd into the project directory, then run "./setup.sh". This will 
automatically compiles the c programs and generate three executables in the 
current directory (handler.elf, backdoor.elf and keylogger.elf).

----------------------
| Step 3 
----------------------
-> Host keylogger executable
host the keylogger.elf on a webserver. The url of the webserver should match
with what is specifed in the setting.json file. Python provies an easy to use
webserver module called 'http.server'. Any other webserver such as nginx, php
would work too.

----------------------
| Step 4
----------------------
-> Run the application
Copy the handler.elf to one linxu machine, and copy the backdoor.ef to another
linux machine. To run the executable, simply type './handler.elf' and 
'./backdoor.elf' 
This last step concludes the basic setup.
-----------------------------------------------------------------------------
</pre>
