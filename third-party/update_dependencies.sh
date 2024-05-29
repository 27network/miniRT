#!/usr/bin/env bash

pushd ./MacroLibX
git pull origin master
make fclean
popd

pushd ./libft
git pull origin main
make fclean
popd

pushd ./tocard-ui
git pull origin main
make fclean
popd
