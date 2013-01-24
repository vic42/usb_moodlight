#!/bin/sh
# This is a shell archive (produced by GNU sharutils 4.9).
# To extract the files from this archive, save it to some FILE, remove
# everything before the `#!/bin/sh' line above, then type `sh FILE'.
#
lock_dir=_sh05207
# Made on 2010-10-27 22:29 CEST by <wzab@wzab>.
# Source directory was `/tmp/atmega'.
#
# Existing files will *not* be overwritten, unless `-c' is specified.
#
# This shar contains:
# length mode       name
# ------ ---------- ------------------------------------------
#   1869 -rw-r--r-- atmega32u2.lib
#
MD5SUM=${MD5SUM-md5sum}
f=`${MD5SUM} --version | egrep '^md5sum .*(core|text)utils'`
test -n "${f}" && md5check=true || md5check=false
${md5check} || \
  echo 'Note: not verifying md5sums.  Consider installing GNU coreutils.'
save_IFS="${IFS}"
IFS="${IFS}:"
gettext_dir=FAILED
locale_dir=FAILED
first_param="$1"
for dir in $PATH
do
  if test "$gettext_dir" = FAILED && test -f $dir/gettext \
     && ($dir/gettext --version >/dev/null 2>&1)
  then
    case `$dir/gettext --version 2>&1 | sed 1q` in
      *GNU*) gettext_dir=$dir ;;
    esac
  fi
  if test "$locale_dir" = FAILED && test -f $dir/shar \
     && ($dir/shar --print-text-domain-dir >/dev/null 2>&1)
  then
    locale_dir=`$dir/shar --print-text-domain-dir`
  fi
done
IFS="$save_IFS"
if test "$locale_dir" = FAILED || test "$gettext_dir" = FAILED
then
  echo=echo
else
  TEXTDOMAINDIR=$locale_dir
  export TEXTDOMAINDIR
  TEXTDOMAIN=sharutils
  export TEXTDOMAIN
  echo="$gettext_dir/gettext -s"
fi
if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null
then if (echo -n test; echo 1,2,3) | grep n >/dev/null
     then shar_n= shar_c='
'
     else shar_n=-n shar_c= ; fi
else shar_n= shar_c='\c' ; fi
f=shar-touch.$$
st1=200112312359.59
st2=123123592001.59
st2tr=123123592001.5 # old SysV 14-char limit
st3=1231235901

if touch -am -t ${st1} ${f} >/dev/null 2>&1 && \
   test ! -f ${st1} && test -f ${f}; then
  shar_touch='touch -am -t $1$2$3$4$5$6.$7 "$8"'

elif touch -am ${st2} ${f} >/dev/null 2>&1 && \
   test ! -f ${st2} && test ! -f ${st2tr} && test -f ${f}; then
  shar_touch='touch -am $3$4$5$6$1$2.$7 "$8"'

elif touch -am ${st3} ${f} >/dev/null 2>&1 && \
   test ! -f ${st3} && test -f ${f}; then
  shar_touch='touch -am $3$4$5$6$2 "$8"'

else
  shar_touch=:
  echo
  ${echo} 'WARNING: not restoring timestamps.  Consider getting and
installing GNU `touch'\'', distributed in GNU coreutils...'
  echo
fi
rm -f ${st1} ${st2} ${st2tr} ${st3} ${f}
#
if test ! -d ${lock_dir} ; then :
else ${echo} "lock directory ${lock_dir} exists"
     exit 1
fi
if mkdir ${lock_dir}
then ${echo} "x - created lock directory ${lock_dir}."
else ${echo} "x - failed to create lock directory ${lock_dir}."
     exit 1
fi
# ============= atmega32u2.lib ==============
if test -f 'atmega32u2.lib' && test "$first_param" != -c; then
${echo} "x - SKIPPING atmega32u2.lib (file already exists)"
else
${echo} "x - extracting atmega32u2.lib (text)"
  sed 's/^X//' << 'SHAR_EOF' > 'atmega32u2.lib' &&
EESchema-LIBRARY Version 2.3  Date: wed, 27 Oct 2010, 22:11:32
#
# ATMEGA32U2
#
DEF ATMEGA32U2 U 0 40 Y Y 1 F N
F0 "U" -600 550 60 H V C CNN
F1 "ATMEGA32U2" 900 -1350 60 H V C CNN
F2 "TQFP32" 900 -1450 60 H I C CNN
F3 "http://www.atmel.com/dyn/resources/prod_documents/doc7799.pdf" -600 550 60 H I C CNN
DRAW
S -700 500 1600 -1250 0 1 0 N
XX XTAL1 1 -1000 400 300 R 50 50 1 1 P
XX (PC0)XTAL2 2 -1000 300 300 R 50 50 1 1 P
XX GND 3 -1000 200 300 R 50 50 1 1 W
XX VCC 4 -1000 100 300 R 50 50 1 1 W
XX (PCINT11/AIN2)PC2 5 -1000 0 300 R 50 50 1 1 B
XX (OC.0B/INT0)PD0 6 -1000 -100 300 R 50 50 1 1 B
XX (AIN0/INT1)PD1 7 -1000 -200 300 R 50 50 1 1 B
XX (RXD1/AIN1/INT2)PD2 8 -1000 -300 300 R 50 50 1 1 B
XX (TXD1/INT3)PD3 9 -1000 -450 300 R 50 50 1 1 B
XX (INT5/AIN3)PD4 10 -1000 -550 300 R 50 50 1 1 B
XX PB6(PCINT6) 20 1900 -850 300 L 50 50 1 1 B
XX D- 30 1900 200 300 L 50 50 1 1 B
XX (XCK/AIN4/PCINT12)PD5 11 -1000 -650 300 R 50 50 1 1 B
XX PB7(PCINT7/OC.0A/OC.1C) 21 1900 -750 300 L 50 50 1 1 B
XX UVCC 31 1900 300 300 L 50 50 1 1 W
XX (~RTS~/AIN5/INT6)PD6 12 -1000 -750 300 R 50 50 1 1 B
XX PC7(INT4/ICP1/CLKO) 22 1900 -650 300 L 50 50 1 1 B
XX AVCC 32 1900 400 300 L 50 50 1 1 W
XX (~CTS~/~HWB~/AIN6/T0/INT7)PD7 13 -1000 -850 300 R 50 50 1 1 B
XX PC6(OC.1A/PCINT8) 23 1900 -550 300 L 50 50 1 1 B
XX (~SS~/PCINT0)PB0 14 -1000 -950 300 R 50 50 1 1 B
XX ~Reset~(PC1/dW) 24 1900 -450 300 L 50 50 1 1 B
XX (SCLK/PCINT1)PB1 15 -1000 -1050 300 R 50 50 1 1 B
XX PC5(PCINT9/OC.1B) 25 1900 -300 300 L 50 50 1 1 B
XX (PDI/MOSI/PCINT2)PB2 16 -1000 -1150 300 R 50 50 1 1 B
XX PC4(PCINT10) 26 1900 -200 300 L 50 50 1 1 B
XX PB3(PD0/MISO/PCINT3) 17 1900 -1150 300 L 50 50 1 1 B
XX UCAP 27 1900 -100 300 L 50 50 1 1 P
XX PB4(T1/PCINT4) 18 1900 -1050 300 L 50 50 1 1 B
XX UGND 28 1900 0 300 L 50 50 1 1 W
XX PB5(PCINT5) 19 1900 -950 300 L 50 50 1 1 B
XX D+ 29 1900 100 300 L 50 50 1 1 B
ENDDRAW
ENDDEF
#
#End Library
SHAR_EOF
  (set 20 10 10 27 22 29 29 'atmega32u2.lib'
   eval "${shar_touch}") && \
  chmod 0644 'atmega32u2.lib'
if test $? -ne 0
then ${echo} "restore of atmega32u2.lib failed"
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} 'atmega32u2.lib': 'MD5 check failed'
       ) << \SHAR_EOF
b8adad0601f3409e237d57af48d7cebe  atmega32u2.lib
SHAR_EOF
  else
test `LC_ALL=C wc -c < 'atmega32u2.lib'` -ne 1869 && \
  ${echo} "restoration warning:  size of 'atmega32u2.lib' is not 1869"
  fi
fi
if rm -fr ${lock_dir}
then ${echo} "x - removed lock directory ${lock_dir}."
else ${echo} "x - failed to remove lock directory ${lock_dir}."
     exit 1
fi
exit 0
