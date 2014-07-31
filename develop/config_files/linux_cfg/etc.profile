

alias rm='mv --verbose -f --backup=numbered --target-directory /tmp/recycle'

export PS1="[\u@`/sbin/ifconfig eth0 | sed -n '0,/^\s\+inet addr:\([0-9]\+[.][0-9]\+[.][0-9]\+[.][0-9]\+\).*$/s//\1/p'` \t \w]\$"



