Tests: Launching the testsuite
================================

In the root directory

```
$ make clean && make debug

```

Then, in test/

```
$ cd test/
$ make clean && make check
```

Tests: Generating coverage
=============================

Requires LCov library / gcovr tool:

```
$ sudo apt-get install gcovr lcov
```

Go to the project root directory:

```
$ make clean && make coverage
```

then

```
$ cd test/
$ make clean && make check
$ cd ../debug
$ gcovr -r .. --html  -o coverage.html
```


