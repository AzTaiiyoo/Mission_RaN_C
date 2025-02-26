# Robot (MRPiZ C) - Makefile non récursif.
# Adapté de celui de Matthias Brun (version de janvier 2024)
# Simplifiée pour simulateur Intox MrPizz par Jérôme Delatour (février 2024)
#
# @author Matthias Brun (matthias.brun@eseo.fr)
# @author Jerome Delatour (jerome.delatour@eseo.fr)


# la librairie MRPiZ permettant de discuter avec le simulateur Intox du robot MrPizz (programme java)
#LIB_MRPIZ = /absolute/path/to/lib_mrpiz/
LIB_MRPIZ = $(realpath  ../../lib_mrpiz-aarch64-v0.5/)
# Avec la ligne précédente, on suppose que :
# - la lib a été mise au même niveau que le répertoire racine (et donc PAS dans le répertoire racine, mais au-dessus),
# - et que son répertoire a été renommée (pour enlever la notion de version)
# Attention : pour ne jamais avoir de soucis avec realpath dans make : 
# ne pas avoir des espaces ou des caractères accentués dans les noms des répertoires et fichiers
# C'est une pratique systématisée dans l'embarqué (et ailleurs aussi)

#
# Organisation du projet.f
#

BINDIR = ../bin
DOCDIR = ../doc
# Définitions des binaires (exécutables) à générer.
EXE_NAME = go
EXE = $(BINDIR)/$(EXE_NAME)

#
# Définitions des outils.
#

# Compilation et édition des liens
#

# Compilateur.
CC = gcc


#Gestion des bibliotheques (intox)
CCFLAGS  = -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12345
CCFLAGS += -I"$(LIB_MRPIZ)/include/mrpiz/"

# pour genérer les dépendances (fichiers .h inclus dans les .c)
CCFLAGS += -MMD -MP

# Soyons portables !
CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE

# Pour le linker
LDFLAGS  = -L"$(LIB_MRPIZ)/lib/" -lintoxmrpiz -lintox
LDFLAGS += -lm
# si besoin de multitache
# LDFLAGS += -lrt -lpthread

# Niveau de vérification
CCFLAGS += -std=gnu11 -Wall -pedantic

#Flags suplémentaires demandés par l'agence française ANSI (Cybersecurité) pour linux avec gcc
CCFLAGS += -fanalyzer -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winit-self -Wstringop-overflow=4 -ftrapv -fstack-protector-strong -fPIE -Wdate-time
LDFLAGS += -fsanitize=undefined -fPIE

# Debug
# Décommentez la ligne qui correspond à votre intention
# Sans debuggage (version dite de production ou "release") :
# CCFLAGS += -O3 -DNDEBUG -D_FORTIFY_SOURCE
# Avec debuggage (version dite de développement):
CCFLAGS += -Og -g -DDEBUG

# Outils de documentation.
DOXYGEN = doxygen
DOXYFILE = Doxyfile
DOXYGENFLAGS =

#
# On prend tous les fichiers .c présents dans src et ses sous répertoires
SRC = $(shell find . -type f -name '*.c')
#on devra en générer un .o
OBJ = $(SRC:.c=.o)
#et verifer ses dépendances
DEP = $(SRC:.c=.d)


#
# Règles du Makefile.
#

.PHONY: all clean doc kill

# Compilation.
all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDFLAGS) -o$(EXE)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

-include $(DEP)

# Nettoyage.
clean:
	@rm -f $(EXE)  $(BINDIR)/core*
	@rm -rf $(DOCDIR)
	@rm -f $(DEP) $(OBJ)

# Génération de la documentation.
doc:
	$(DOXYGEN) $(DOXYGENFLAGS) $(DOXYGENCONF)

#
# Arrêt du programme
# (utile pour utilisation dans certains IDE)#

kill:
	killall -s INT "$(EXE)"	

