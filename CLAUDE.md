# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project purpose

DataBaseViewer is a personal learning project: a pgAdmin-style database client (connect to a DB locally or remotely via SSH tunnel — e.g. a Postgres instance in Docker — browse tables, and do CRUD through a GUI). The real goal is for the author to re-learn modern C++ and Qt before an upcoming internship, not to ship a finished tool quickly.

The full roadmap (6 phases, 15 GitHub issues/milestones) and a class-by-class object-oriented architecture proposal live in `docs/TP_DataBaseViewer.pdf`. Read it before planning any non-trivial change — it defines the intended class breakdown and the order features are meant to be built in.

## How to collaborate on this repo

**Act as a guide, not an implementer, for the core application logic.** When asked to work on a feature from the roadmap (connection handling, models/views, CRUD, SQL console, SSH tunnel, etc.), prefer explaining the relevant Qt classes/patterns, pointing out pitfalls, and reviewing code over writing the full implementation — the point of the project is for the author to write that code themselves. Small illustrative snippets (class skeletons, a few lines showing an API) are fine; full working features are not, unless explicitly asked for.

This restriction does not apply to incidental project scaffolding the author has said is out of scope for their learning (e.g. `README.md`, `CLAUDE.md`, `.gitignore`, build config) — write those directly when asked.

## Build

Qt 6, C++17, qmake project (`DataBaseViewer.pro`), modules: `widgets sql`.

```
qmake DataBaseViewer.pro
mingw32-make      # or make / nmake depending on platform
```

Or open `DataBaseViewer.pro` in Qt Creator and build from there. There is no test suite and no lint config in the repo yet.

## Architecture

The codebase is currently just the default Qt Creator Widgets skeleton (`main.cpp`, `mainwindow.h/.cpp/.ui`) — a single empty `MainWindow`. No database, connection, or model/view code exists yet.

The intended architecture (detailed in `docs/TP_DataBaseViewer.pdf`) separates the app into these responsibilities, none of which exist yet but should be created as the roadmap issues are implemented:

- `ConnectionInfo` — value struct for connection parameters (engine, host, port, credentials, SSH tunnel options).
- `DatabaseConnection` — the only class allowed to touch `QSqlDatabase` directly; wraps open/close/error state and an optional `SshTunnel`.
- `SshTunnel` — manages a port-forwarding tunnel's lifecycle for remote/Docker connections.
- `ConnectionProfileStore` — persists named connection profiles via `QSettings`.
- `ConnectionDialog` — collects form input and produces a `ConnectionInfo`; knows nothing about how connections are opened.
- `TableExplorerWidget` — lists tables for a connection, emits the selected table name.
- `TableDataWidget` — displays/edits a table's rows (`QSqlTableModel`-based CRUD, filter, sort).
- `SqlConsoleWidget` — free-form SQL execution and result display.
- `DatabaseSessionWidget` — composes the above per open connection; one instance per tab.
- `MainWindow` — owns a `QTabWidget` of `DatabaseSessionWidget`s plus the connection dialog; orchestration only, no business logic.

The key invariant: UI-facing widgets never talk to `QSqlDatabase` or SSH directly, only to `DatabaseConnection`. Keep new code aligned with this separation rather than putting logic directly in `MainWindow`.
