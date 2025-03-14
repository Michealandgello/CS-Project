# CS 3502 Project 1 - Multi-Threaded Programming and IPC
--
## Setup Instructions

To run this project, you'll need the following setup:

### 1. **Virtual Machine (VM) Setup**
   - **VMware**: I used VMware to create a virtual machine.
   - **Ubuntu ISO**: I installed Ubuntu 24.04.1 Desktop (64-bit) as the operating system.

### 2. **Development Tools**
   - **Visual Studio Code (VS Code)**: Download and install VS Code from the [official website](https://code.visualstudio.com/) or the Ubuntu Software Center.
   - **Extensions**:
     - **C/C++**: Provides IntelliSense, debugging, and code browsing for C/C++.
     - **C/C++ Extension Pack**: Includes additional tools for C/C++ development.
     - **Code Runner**: Allows you to run code snippets directly in the terminal.

### 3. **Dependencies**
   - **.NET Runtime and APP Host**: Install the latest versions of .NET runtime and APP host (I used version 9.0.2).
     ```bash
     sudo apt-get update
     sudo apt-get install dotnet-sdk-9.0
     ```
   - **GCC Compiler**: Install the GNU Compiler Collection (GCC) to compile C programs.
     ```bash
     sudo apt-get install build-essential
     ```

### 4. **Cloning the Repository**
   Clone this repository to your local machine:
   ```bash
   git clone https://github.com/your-username/your-repo-name.git
   cd your-repo-name
