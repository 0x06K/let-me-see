# 🕵️‍♂️ let-me-see — Windows Internals CLI Tool

**let-me-see** is a lightweight and sneaky CLI utility that gives you deep insight into Windows internals. From processes and threads to memory, handles, tokens, and the PEB.

---

## ⚙ Features

- 🔍 Enumerate running processes and modules
- 👤 Inspect tokens, threads, and handles
- 🧠 Read arbitrary memory of a target process
- 🧱 Dump heap and PEB structures
- 🧵 Set and switch between PIDs in session

---

## 🚀 Usage

Start the CLI:
```bash
main.exe
````

You'll see:

```
<=== LET ME SEE ^_^ ===>
Type 'help' to see available commands.
cli>
```

### 🔧 Available Commands

| Command                | Description                          |
| ---------------------- | ------------------------------------ |
| `set pid <pid>`        | Set the target process ID            |
| `peb`                  | Enumerate the PEB of the current PID |
| `threads`              | List threads of the target process   |
| `modules`              | List loaded modules (DLLs)           |
| `heap`                 | Analyze heap structures              |
| `handles`              | List open handles of the process     |
| `tokens`               | Dump token privileges and details    |
| `process`              | List all running processes           |
| `help`                 | Show command usage                   |
| `exit`       | Exit the CLI                         |

---

## 💻 Example

```shell
cli> set pid 1234
cli> threads
cli> tokens
```

---

## 🔐 Requirements

* Windows 10/11
* Run as **Administrator** (recommended)
* `SeDebugPrivilege` is required for inspecting other processes

---

## 📦 Build

Use any C compiler (like MSVC or MinGW):

```bash
gcc src\*.c main.c -o main.exe
```

---

## 📁 Project Structure

```
let-me-see/
├── main.c        # Main CLI interface
├── modules.c           # Module enumeration
├── handles.c           # Handle analysis
├── tokens.c            # Token and privileges
├── heap.c              # Heap structures
├── threads.c           # Thread inspection
├── process.c           # Process listing
├── EnumeratePEB.c      # PEB parsing logic
```

---


## ⚠️ Disclaimer

This tool is for **educational and research purposes only**. Do not use against systems without permission. You are responsible for your actions.

---

---

## 🕳️ Future Ideas

* Inject shellcode into remote process
* Interactive memory explorer
* Hook detection
* Live handle monitoring
* Export reports to JSON

---

## 💬 The One Who'll Never Leave You — Is You
> *Start there ;)*

