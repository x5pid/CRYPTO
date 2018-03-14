# Extraits de "An Introduction to Makefiles" 
# https://www.gnu.org/software/make/manual/html_node/Introduction.html#Introduction
# ----------------------------
# The make utility automatically determines which pieces of a large program need to be recompiled, and issues commands to recompile them
# You need a file called a makefile to tell make what to do. Most often, the makefile tells make how to compile and link a program.
# A simple makefile consists of “rules” with the following shape: 
# target : prerequisites …
#        recipe
#
# A target is usually the name of a file that is generated by a program -> cible
# A prerequisite is a file that is used as input to create the target. -> dépendance
# A recipe is an action that make carries out
#------------------------
# Règles implicites
# -----------------------
# règle pour obtenir .o à partir de .c
# %.o: %.c
#       $(CC) -c $(CFLAGS) -o $@ $<

# règle pour obtenir l'exécutable à partir de .o
# %: %.o
#       $(CC) -o $@ $< $(LDLIBS)
#
# -----------------------
# Variables utilisées
# -----------------------
# $@  - nom de la cible
# $<  - première dépendance
# $?  - toutes les déps plus récentes que la cible
# $^  - toutes les déps

CC=gcc
CFLAGS= -Werror -Wall -g
LDLIBS=

all: q1

clean:
	$(RM) q1 *.o
