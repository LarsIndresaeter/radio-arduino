# versioning

this project is using semantic versioning, [semver.org](https://www.semver.org).

## git tags

Git tags are used to describe major, minor and patch number in the format
`major.minor.patch`. The [conan](./conan.md) files have a function for
calculating the version string on the format
`major.minor.patch.commits_not_in_main+gitsha`. If the code used to build is
not commited then `-dirty` is appended. This format agrees with demanding formatting.

For example, if the latest tag is `0.4.0` and 3 commits have been made since the
tag was generated and files used to build are not commited then the version
string will look like this `0.4.0.3+5aa872e-dirty`.

## where the version number is used

Conan packages will use the version number, like this `raduino-gateway/0.4.0.3+5aa872e-dirty@raduino/test`.

The gateay or node can respond with the version it has using the command.

```console
./bin/raduino-system-commands -v
```

which will resulst in a response similar to this one.

```console
complete : GET_VERSION            :  versionString="0.4.0.3+31e4e1c"
```

## updating version numbers

The raduino tool can deal with the version tags for you.

```console
raduino release <action> :
           bump <number> : bump number <major|minor|patch> and create tag
                    push : push latest tag to origin
                  detect : detect change but do not create tag
             detect auto : detect change and create tag
                  latest : print latest tag
```

| command                     | description                                      |
| --------------------------- | ------------------------------------------------ |
| raduino release latest      | print latest version tag                         |
| raduino release bump major  | create a new tag with incremented major version  |
| raduino release bump minor  | create a new tag with incremented minor version  |
| raduino release bump patch  | create a new tag with incremented patch version  |
| raduino release detect      | detect change and suggest action                 |
| raduino release detect auto | detect change and automatically create a new tag |
| raduino release push        | push tag to the remote git server                |

## conventional commits

[conventional commits](https://www.conventionalcommits.org/en/v1.0.0/) are used to
detect which version number to increase. This project is mostly using these prefixes.

| prefix          | description                           |
| --------------- | ------------------------------------- |
| BREAKING CHANGE | increase major number                 |
| feat            | feature, increase minor number        |
| fix             | fix, increase patch number            |
| docs            | changes to the documentation          |
| chore           | changes not affecting code            |
| refactor        | changes not changing behavior of code |

