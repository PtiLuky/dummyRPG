# Contributing to DummyRPG

Help welcome in:

* User feedbacks
* Communication
* Documentation
* Coding tasks.


## User feedbacks

Any feedback to understand what can be improved in this project is welcome.

To express yourself please chat with us on Discord or open a new Issue ticket on GitHub.


## Communication

*(EN or FR)*

Help to manage a [Discord server](https://discord.gg/qzx4AjT), keep people informed with new updates, chat with developers to
make the link between the users, the community and the dev.  
And make the community grow!


## Documentation

*(EN or FR)*

You don't know how/don't want to code? But still want to help?

We need you to improve documents as this one!

* Help the newcomers:
  * new users with help on How to use the Editor to create a game :)
  * contributors by provinding guidelines and how to find their place in this project
* Translate existing document to EN or FR


## Coding

So many things to do here :)

Please get in touch with us if any of the following point is not clear enough.

First make sure you understand [the specifications page (french)](doc/specifications.md).  
Then [Install the project](doc/install.md).


#### Coding Style

Modern C++ (no new, delete and C-like casts).  
Most important is to fit in the current document coding-style.

Setup your IDE to use the **.clang-format** file to automatically apply the right formatting style.  
clang-format should be installed and the added to your IDE (exists as QtCreator and Visual Studio addons).


#### Continuous Integration (CI)

A CI with GitHub Actions is set up.

It checks on each commits and PR:

* Compilation success on Windows
* Compilation success on Linux
* Send results to SonarCloud analysis to keep track of technical debt

What should be done:

* Check test results (it runs them without checking results...)
* Check code coverage

#### Help wanted

* Coding (see current issues on GitHub)
* Testing
* CI improvement
