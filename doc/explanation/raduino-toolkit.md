# raduino toolkit

This repository has a tool named `raduino` which is a one-stop-tool for all you
need to do for developement and usage.

## source step

To use the tool you must source it with the command `source tools/setup.sh`.

## help menu

```console
raduino build menu
    format <code>
    lint <doc>
    dockerapp <build|run>
          build or run a container with raduino apps
    devbox start up a container where you can build and run this software
    tool <personalize|gateway|node|test|mqtt|power>
    setup <arduino|interface|linux|all>
          set up build area using conan and cmake
    clean <arduino|interface|linux|all>
          clean build area
    build <arduino|interface|linux|all>
          build target in area created by setup
    all
          run commands: clean, setup and build
    release <bump|push|detect|latest>
    flash <rf-nano|nano-pro>
          flash arduino
    mqtt
          mqtt commands
    conan
          create conan packages
```
