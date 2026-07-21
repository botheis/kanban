# kanban
Because it's usefull, and I have no restriction on using it, and I don't have to pay for it, and I can customize as I want.

## Commands

Here is the list of Makefile commands

### make

Build all the solution:
```bash
make
# OR
make all
# OR
make build
```
These three commands do the same thing: it builds everithing in the build folder.

### make lib

make lib generates the dependencies libs. The dependencies are ar archives (.a)
```bash
make lib
```

### make test

make test generates all the test executables.
```bash
make test
```

### make console

make console generates the commandline app. With this it is possible to command the kanban api through commandlines.
```bash
make console
```

### make app

make app generates the GUI app. It is probably more convenient to use a kanban in graphical mode.

```bash
make app
```

### make version

make version display the project info such as the versions, license, current hash.

```bash
make version
```

### make clean

make clean deletes the build folder used to build everything.
```bash
make clean
```

### make display

make display displays the make variables used to build everything. It's more usefull on debugging Makefile than everything else.
```bash
make display
```
