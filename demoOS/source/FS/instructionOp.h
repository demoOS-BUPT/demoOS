#pragma once

#ifndef INSTRUCTIONOP_H
#define INSTRUCTIONOP_H

#include<iostream>
#include<string>
#include<cmath>
#include<new>
#include<cstdlib>
#include<cstring>
#include"other.h"

using namespace std;

void mkdir_instruction(string op);

void touch_instruction(string op);

void cp_instruction(string op);

void mv_instruction(string op);

void ln_instruction(string op);

void rm_instruction(string op);

void cat_instruction(string op);

void vim_instruction(string op);

void ls_instruction(string op);

void su_instruction(string op);

void chmod_instruction(string op);

void chown_instruction(string op);

void chgrp_instruction(string op);

void whoami_instruction(string op);

#endif // 
