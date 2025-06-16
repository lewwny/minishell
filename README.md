CHECKLIST de ce qui reste a faire :
-Probleme gestion de quotes, closed ou unclosed, + leaks si on les manipule.
''' puis '''' = leaks
-Probleme avec les multi here-doc (pas tres grave, mais a regler)
-defaut avec les signaux (pas tres tres grave non plus)
-GROS PROBLEMES AVEC L'EXPAND (fonctionne tres tres bizarrement)
-Exit_code for /
-echo \n hola ??
-""''echo hola""'''' que""'' tal""''
- echo > < puis une commande qui n'existe pas = leak parsing
-echo "'"h'o'la"'" returned :' h o la ' expected: 'hola'
-petites modifications dans export (sans param, et sans =)
-gestion + et - dans exit