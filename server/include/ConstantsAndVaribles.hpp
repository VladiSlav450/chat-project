// server/incldue/ConstantsAndVaribles.hpp

#ifndef CONSTANTSANDVARIBLES_HPP
#define CONSTANTSANDVARIBLES_HPP

// FOR FORK-PROCESSING

#define WORKERS_COUNT 3
#define STREAMS_COUNT 2
#define SOCKET_PARENT 0
#define SOCKET_CHILD 1

enum {
    max_line_length = 1023,
    qlen_for_listen = 16
};

// FOR CHAT 

static const char welcom_msg[] = "Welcom to the Chat! You are known as ";
static const char entered_msg[] = " has entered the chat";
static const char left_msg[] = " has left the chat";

static const char server_commands[] = "<server command>";
static const char server_shutdown[] = "server shutdown";

static const char new_name_msg[] = "enter a new name:";
static const char invalid_name_msg[] = "Invalid name. The name must be less than 10 characters long"
                                       " and contain only letters or numbers."
                                       "Try again:\n";
static const char name_already_take_msg[] = "Name already take. Choose another.\n";                                       

static const char what_commands_are_there[] =  "available commands: /help /users /name_users /change_name /quit /shutdown";
static const char unknow_command_msg[] = "unknow command. Write /help for commands list."; 


// FOR SITE
      
static const char PATHCONFIGFILE[] = "../lib/config.conf";

#endif // CONSTANTSANDVARIBLES_HPP
