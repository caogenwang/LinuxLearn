#!bin/bash
declare -r TRUE=0
declare -r FALSE=1
declare -r PASSWD_FILE=/etc/passwd

function to_lower(){
    local str="$@"
    local output
    output=$(tr '[A-Z]' '[a-z]'<<<"${str}")
    echo $output
}

function is_root(){
    [ $(id -u) -eq 0 ] && return $TRUE || return $FALSE
}

function is_user_exits(){
    local u=$1
    grep -q "^${u}" $PASSWD_FILE && return $TRUE || return $FALSE
}

