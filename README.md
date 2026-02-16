<div id="top" align="center">
<h1>FileSorter 📂</h1>
<p>cross-platform application to keep your Download folder organized</p>

![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/Zheng-Bote/file-sorter?logo=GitHub)](https://github.com/Zheng-Bote/file-sorter/releases)
<br/>
[Report Issue](https://github.com/Zheng-Bote/file-sorter/issues) · [Request Feature](https://github.com/Zheng-Bote/file-sorter/pulls)

</div>

<hr>

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Description](#description)
  - [✨ Key Features](#-key-features)
  - [Status](#status)
- [Documentation & Screenshots](#documentation--screenshots)
  - [Usage](#usage)
  - [⚙️ Configuration](#-configuration)
    - [Command Line Options](#command-line-options)
  - [Screenshots](#screenshots)
    - [Autostart Win11](#autostart-win11)
- [🚀 Getting Started](#-getting-started)
  - [Windows](#windows)
    - [Setup](#setup)
  - [Linux](#linux)
  - [Build](#build)
    - [Build Instructions](#build-instructions)
    - [Project Structure](#project-structure)
  - [🏗️ Architecture](#-architecture)
    - [Component Overview](#component-overview)
    - [Class Diagram](#class-diagram)
    - [Component Diagram](#component-diagram)
    - [Use Case Diagram](#use-case-diagram)
    - [Sequence Diagram](#sequence-diagram)
    - [State Machine Diagram](#state-machine-diagram)
    - [Deployment Diagram](#deployment-diagram)
    - [Sorting Flow Logic](#sorting-flow-logic)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)
- [👤 Author](#-author)
  - [Code Contributors](#code-contributors)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

<hr>

# Description

![Language](https://img.shields.io/badge/language-C%2B%2B23-00599C.svg)
![Framework](https://img.shields.io/badge/framework-Qt6-41CD52.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**FileSorter** is a robust, cross-platform desktop application designed to keep your Download folder organized. Built with modern **C++23** and **Qt6**, it automatically detects new files and sorts them into subdirectories based on user-defined rules.

## ✨ Key Features

- **Real-time Monitoring:** Uses `QFileSystemWatcher` to detect new files immediately.
- **Smart Debouncing:** Waits for downloads to finish (via `QTimer`) before moving files to avoid corruption.
- **Flexible Rules:** Map specific file extensions (e.g., `pdf, docx`) to custom folders.
- **Date-based Sorting:** Optional setting per rule to organize files into `YYYY/MM/DD` substructures.
- **Hot-Reloading Rules:** Changes to the configuration are applied immediately, even while monitoring is active.
- **Background Mode:** Start the application minimized via CLI (ideal for system autostart).
- **Cross-Platform:** Runs seamlessly on Windows, macOS, and Linux.
- **Internationalization (i18n):** Automatically switches between **English** and **German** based on system settings.
- **Persistent Configuration:** Rules are saved automatically using `QSettings` (Registry/Ini/Plist).

## Status

![GitHub Created At](https://img.shields.io/github/created-at/Zheng-Bote/file-sorter)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/Zheng-Bote/file-sorter?logo=GitHub)](https://github.com/Zheng-Bote/file-sorter/releases)
![GitHub Release Date](https://img.shields.io/github/release-date/Zheng-Bote/file-sorter)
![Status](https://img.shields.io/badge/Status-stable-green)

![GitHub Issues](https://img.shields.io/github/issues/Zheng-Bote/file-sorter)
![GitHub Pull Requests](https://img.shields.io/github/issues-pr/Zheng-Bote/file-sorter)

---

# Documentation & Screenshots

## Usage

## ⚙️ Configuration

1. Launch the application.
2. Click "+" to add a new rule.
3. Folder Name: Enter the target subfolder name (e.g., Images).
4. Extensions: Comma-separated list (e.g., jpg, png, svg).
5. Use Date?: Check this box to organize files by year/month/day within the target folder.
6. Enable "Automatic Monitoring" to let the app work in the background.

To remove a rule, select the row and click "-".

> \[!NOTE]
> You can modify rules while "Automatic Monitoring" is active. The changes will be applied immediately.

### Command Line Options

You can start the application via the command line (or a shortcut) to launch it directly in the background with monitoring enabled.

| Option              | Description                                                                                      |
| :------------------ | :----------------------------------------------------------------------------------------------- |
| `-m`, `--minimized` | Starts the application minimized in the taskbar and automatically enables the folder monitoring. |

**Example:**

```bash
FileSorter.exe --minimized
```

## Screenshots

![App Screenshot](/docs/img/01_de.png)
![App Screenshot](/docs/img/about_de.png)

### Autostart Win11

![App Screenshot](/docs/img/win11_autostart_de.png)

---

([back to top](#top))

# 🚀 Getting Started

## Windows

### Setup

There are two options:

1. Unzip

   -> no Administrator rights needed

- unzip `FileSorter-1.2.1-win64.zip` and execute `FilesSorter.exe`

2. Setup

   -> no Administrator rights needed (optional)

- execute `FileSorter_x86_amd64_v1.2.1_setup.exe` and follow the instructions.
- doubleclick `FileSorter.exe` or your Desktop-Shortcut

## Linux

Just download the AppImage and make it executable:

```Bash
chmod +x FileSorter-1.2.1-x86_64.AppImage
sudo cp FileSorter-1.2.1-x86_64.AppImage /usr/local/bin/
```

And copy the desktop file to your desktop (Path depends on your Linux distribution):

```Bash
sudo cp resources/FileSorter.desktop ~/.local/share/applications/
```

And add it to your autostart (Path depends on your Linux distribution):

```Bash
sudo cp resources/FileSorter_autostart.desktop ~/.config/autostart/
```

And copy the app_icon (Path depends on your Linux distribution):

```Bash
sudo cp resources/app_icon.png /usr/local/share/icons/hicolor/256x256/apps/FileSorter.png
```

## Build

**_Prerequisites_**

- C++ Compiler supporting C++23 (GCC 13+, Clang 16+, MSVC 2022 v17.6+)
- CMake (3.23 or newer)
- Qt 6 (Core, Gui, Widgets, LinguistTools)

### Build Instructions

```Bash
# 1. Clone the repository
git clone [https://github.com/Zheng-Bote/file-sorter.git](https://github.com/Zheng-Bote/file-sorter.git)
cd file-sorter

# 2. Create build directory
mkdir build && cd build

# 3. Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# 4. Build
cmake --build . --config Release

# 5. Installer packages (optional, Windows only)
cpack.exe -C Release
# in some cases (if Chocolatey is installed on your system) the complete path to your Qt cpack is needed
& "C:\Qt\Tools\CMake_64\bin\cpack.exe" -C Release
```

### Project Structure

```Plaintext
file-sorter/
├── CMakeLists.txt       # Build configuration
├── resources.qrc        # Resource file (icons, etc.)
├── resources/           # Resource files (icons, logos, for AppImage etc.)
├── include/             # Header files (*.hpp)
├── src/                 # Source files (*.cpp)
├── translations/        # Translation files (*.ts)
└── configure/           # CMake configuration scripts
```

## 🏗️ Architecture

The application follows a clean separation of concerns, splitting the User Interface (UI) from the business logic.

### Component Overview

1.  **MainWindow (UI):** Handles user interaction, configuration (TableWidget), and displays logs. It manages the application lifecycle and processes optional CLI arguments (QCommandLineParser) to handle minimized starts.
2.  **FileSorter (Logic):** A `QObject` based worker class. It handles:
    - File system monitoring (`QFileSystemWatcher`).
    - Debouncing logic to wait for file write operations.
    - The actual sorting algorithm (Pattern matching & `std::filesystem`/`QDir` operations).
3.  **Config & Resources:**
    - `rz_config.hpp`: Generated by CMake for versioning and metadata.
    - `QSettings`: Stores sorting rules persistently.
    - `Qt Linguist`: Handles translations (`.ts` / `.qm`).

### Class Diagram

```mermaid
classDiagram
    class MainWindow {
        +QTableWidget* m_table
        +QTextEdit* m_logOutput
        +QCheckBox* m_autoSortCheck
        -setupUI()
        -saveSettings()
        -loadSettings()
        -onRulesModified()
    }

    class FileSorter {
        +void sortDownloads(List categories)
        +void setMonitoring(bool enable)
        +void updateRules(List categories)
        -QFileSystemWatcher* m_watcher
        -QTimer* m_debounceTimer
        -onDirectoryChanged()
    }

    class Category {
        +QString folderName
        +QStringList extensions
        +bool useDatePath
    }

    class AboutDialog {
        +AboutDialog(QWidget* parent)
    }

    MainWindow *-- FileSorter : owns
    MainWindow ..> Category : creates
    MainWindow ..> AboutDialog : shows
    FileSorter -- QFileSystemWatcher : uses
    FileSorter -- QTimer : uses
```

### Component Diagram

```mermaid
graph TD
    UI["MainWindow (UI)"]
    Logic["FileSorter (Logic)"]
    Config["QSettings"]
    Watcher["QFileSystemWatcher"]
    Logger["Log System"]

    UI -->|controls| Logic
    UI -->|reads/writes| Config
    UI -->|displays| Logger
    Logic -->|uses| Watcher
    Logic -->|reads| Config
    Logic -->|writes| Logger
```

### Use Case Diagram

```mermaid
graph TD
    User((User))
    OS((Operating System))

    subgraph FileSorter [FileSorter System]
        UC1(Configure Rules)
        UC2(Monitor Downloads)
        UC3(Sort Files)
        UC4(Log Activities)
    end

    User --> UC1
    OS --> UC2
    UC2 --> UC3
    UC3 --> UC4
    User --> UC4
```

### Sequence Diagram

```mermaid
sequenceDiagram
    participant OS as Operating System
    participant Watcher as QFileSystemWatcher
    participant Sorter as FileSorter
    participant Timer as QTimer
    participant FS as File System

    OS->>Watcher: Directory Changed Signal
    Watcher->>Sorter: onDirectoryChanged()
    Sorter->>Timer: start(2000ms)
    Note over Sorter, Timer: Debouncing...
    Timer-->>Sorter: timeout()
    Sorter->>Sorter: sortDownloads()
    Loop For each file
        Sorter->>FS: Check file extension
        Alt Match Rule
            Sorter->>FS: Move file to target
            Sorter->>Sorter: Log Success
        Else No Match
            Sorter->>Sorter: Skip
        End
    End
```

### State Machine Diagram

```mermaid
stateDiagram-v2
    [*] --> Idle
    Idle --> Monitoring : Enable Auto-Sort
    Monitoring --> Debouncing : File Change Detected
    Debouncing --> Debouncing : New Change (Reset Timer)
    Debouncing --> Sorting : Timer Timeout
    Sorting --> Monitoring : Sorting Complete
    Monitoring --> Idle : Disable Auto-Sort
    Idle --> [*]
```

### Deployment Diagram

```mermaid
graph TD
    subgraph Workstation [User Workstation]
        subgraph OS [Operating System]
            FS[File System]
            App[FileSorter App]
        end
    end
    App -->|Reads/Writes| FS
```

### Sorting Flow Logic

When Automatic Monitoring is enabled, the application follows this process:

```mermaid
flowchart TD
    A["Start / New File Detected"] --> B{"Is Debounce Timer Active?"}
    B -- Yes --> C["Reset Timer"]
    B -- No --> D["Start Timer (e.g. 2s)"]
    C --> E["Wait..."]
    D --> E
    E --> F{"Timer Timeout"}
    F --> G["Scan Download Directory"]
    G --> H{"File matches Rule?"}
    H -- No --> I["Skip File"]
    H -- Yes --> J{"Date Sorting Enabled?"}
    J -- Yes --> K["Create Path: Folder/YYYY/MM/DD"]
    J -- No --> L["Create Path: Folder/"]
    K --> M["Move File"]
    L --> M
    M --> N["Log Action"]
```

---

([back to top](#top))

# 🤝 Contributing

Contributions are welcome! Please fork the repository and create a pull request.

1. Fork the Project
2. Create your Feature Branch (git checkout -b feature/AmazingFeature)
3. Commit your Changes (git commit -m 'Add some AmazingFeature')
4. Push to the Branch (git push origin feature/AmazingFeature)
5. Open a Pull Request

# 📄 License

Distributed under the MIT License. See LICENSE for more information.

Copyright (c) 2025 ZHENG Robert

# 👤 Author

[![Zheng Robert - Core Development](https://img.shields.io/badge/Github-Zheng_Robert-black?logo=github)](https://www.github.com/Zheng-Bote)

## Code Contributors

![Contributors](https://img.shields.io/github/contributors/Zheng-Bote/qt-desktop_file_encryption-decryption?color=dark-green)

<hr>
([back to top](#top))

:vulcan_salute:
