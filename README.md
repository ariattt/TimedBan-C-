# Timed Ban

Block certain sites (e.g. YouTube) for a period of time so that you can focus on your valueable work! A perfect use case is shutting down attention attraction website like YouTube.

## Usage

Clone the repository and cd into that directory. Run
```
make ban [time in seconds] [lists of sites...]
```
e.g. this will ban YouTube and Quora for the next 60 minutes.
```
make ban 3600 youtube.com quora.com
```

## Active Development
WIP:
    * support more unit, e.g. minutes, hours
    * handle some signals so it cannot be killed easily
    * change makefile to make it run in the background
    * rewrite in other fancier languages, e.g. python/go. C++ is bit of wordy and inconsistent (boost algo to others) here.
    * upload it to a package manager

Feel free to suggest any other features you want! 


