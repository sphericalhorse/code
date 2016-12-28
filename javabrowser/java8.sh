#!/bin/bash

DIR=$(dirname "$0")
ENV_HOME="$DIR/java8"
FIREFOX_HOME="$ENV_HOME/firefox"
PROFILE_HOME="$ENV_HOME/firefox-profile"
mkdir -p "$PROFILE_HOME"

_JAVA_HOMES=( $ENV_HOME/jre1.8* )
export JAVA_HOME="${_JAVA_HOMES[0]}"

if [ ! -e "$FIREFOX_HOME/browser/plugins/libnpjp2.so" ]
then
	ln -rs "$JAVA_HOME/lib/i386/libnpjp2.so" "$FIREFOX_HOME/browser/plugins/libnpjp2.so"
fi

$FIREFOX_HOME/firefox --no-remote --profile "$PROFILE_HOME"
