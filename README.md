# CPPRT

# Qt Chat Application

A modern chat application built with Qt that enables real-time communication between multiple users. The project consists of both a server and a client application.

## Features

### Server Application
- TCP-based chat server handling multiple client connections
- Real-time user management (connection/disconnection)
- Broadcast and private message routing
- Administrative interface to monitor and manage connected users
- Support for "Wizz" notifications

### Client Application
- Modern and intuitive user interface
- Real-time messaging in general chat
- Private messaging with tabbed conversations
- Emoji support with categorized picker
- "Wizz" effect with animation and sound
- Message display with stylized bubble design
- Online users list
- Shift+Enter for new line in messages

## Technical Requirements

- Qt 6.0 or higher
- CMake 3.16 or higher
- C++17 compatible compiler
- Operating System: Windows or macOS

## Project Structure

```
.
├── Client/
│   ├── HeaderFiles/         # Client header files
│   ├── SourceFiles/        # Client source files
│   ├── UIFiles/           # Qt UI files
│   └── resources/         # Client resources (sounds, etc.)
├── Server/
│   ├── HeaderFiles/         # Server header files
│   ├── SourceFiles/        # Server source files
│   └── UIFiles/           # Server UI files
└── .env                   # Environment configuration
```

## Building and Running

1. Clone the repository:
```bash
git clone [https://github.com/oussema-fatnassi/CPPRT.git]
```

2. Create a `.env` file in the root directory with the following content:
```
CHAT_SERVER_IP= ( Your IPv4 address here)
CHAT_SERVER_PORT=12345
```

3. Build the server:
```bash
cd Server
mkdir build && cd build
cmake ..
cmake --build .
```

4. Build the client:
```bash
cd Client
mkdir build && cd build
cmake ..
cmake --build .
```

5. Run the server first, then launch client instances.

## Implementation Details

### Server Components
- `ChatServer`: Handles TCP connections and message routing
- `ServerMainWindow`: Administrative interface for server management

### Client Components
- `ChatClient`: Manages server communication
- `MainWindow`: Main application window and component coordination
- `MessageHandler`: Message processing and display
- `TabManager`: Conversation tab management
- `UIEventHandler`: User interface event handling
- `EmojiManager`: Emoji selection and insertion
- `WizzManager`: Wizz effect coordination
- `MessageBubbleManager`: Custom message display styling

## Communication Protocol

Messages are sent in text format with the following prefixes:
- `NAME:` - Username registration
- `BROADCAST:` - General chat messages
- `DM:` - Private messages
- `WIZZ:` - Wizz effect notifications
- `USERS:` - Online users list updates



## Developed by :

[Baptiste Appriou, Oussema Fatnessi and Ali Abakar Issa]
