# Architecture Diagrams

This file contains the Mermaid source code for the architecture diagrams used in the project documentation.

## Use Case Diagram

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

## Component Diagram

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

## Sequence Diagram

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

## State Machine Diagram

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

## Deployment Diagram

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
