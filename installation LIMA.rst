Contexte:
tango v.7...
archi: Linux 2.6.32-41-generic #90-Ubuntu SMP Tue May 22 11:31:25 UTC 2012 i686 GNU/Linux
camera Basler Scout (non plugged)

Pr�requis :
- python 2.6.5 installed
- gcc 4.4.3
- git 1.7.0.4

Actions (p9-11 doc http://lima.blissgarden.org/latex/Lima.pdf):
- Pylon:
get pylon-3.2.1-x68.tar.gz
d�compresser dans /opt
/opt/pylon/pylon-setup-env.sh /opt/pylon
remarque: dans le fichier INSTALL du tar.gz, line 74 : le saut de ligne apr�s "export LD_LIBRARY_PATH=$..." devrait �tre supprim�
source /opt/bin/pylon-setup-env.sh

git clone --recursive git://github.com/esrf-bliss/Lima.git
cd Lima
make (ne fonctionne pas avant l'�dition de config.inc)
�diter config.inc et �crire "COMPILE_CORE=1
COMPILE_SIMULATOR=0
COMPILE_SPS_IMAGE=1
COMPILE_ESPIA=0
COMPILE_FRELON=0
COMPILE_MAXIPIX=0
COMPILE_PILATUS=0
COMPILE_BASLER=1
COMPILE_CBF_SAVING=0
export COMPILE_CORE COMPILE_SPS_IMAGE COMPILE_SIMULATOR \
       COMPILE_ESPIA COMPILE_FRELON COMPILE_MAXIPIX COMPILE_PILATUS \
       COMPILE_BASLER COMPILE_CBF_SAVING"
	   
Doit-on laisser COMPILE_CONFIG=1 dans config.inc ?
=> Non, ce n'est pas obligatoire mais tr�s utile donc c'est par d�faut. Voire les  attributs/propri�t�ss/commandes commen�ant par �config� (http://lima.blissgarden.org/applications/tango/doc/index.html#main-device-limaccds)

make config
Quand on ex�cute �make config� dans le r�pertoire Lima : �usr/bin/sip: not found� -> installer le paquet SIP4 (ne semble pas �tre indiqu� dans la doc)
=> sera ajout� dans la doc
make install
Quand on ex�cute �make� dans le r�pertoire Lima : �../core/src/GslErrorMgr.cpp:24:27: error: gsl/gsl_errno.h: Aucun fichier ou dossier de ce type� -> install libgsl0-dev (ne semble pas �tre indiqu� dans la doc)

=> Quand on ex�cute  �make� dans le r�pertoire Lima : un grand hombre d'alertes sans lien (un switch g�re des cas avec un return apr�s �tre sorti du switch et des alertes #pragma sur des d�finitions qui sont obsol�tes)
make -C sip -j3

export INSTALL_DIR=~/SW_tango/
make config

Apr�s, p11, se d�placer dans Lima/application/tango/.