## HTMAKE - Program to generate a makefile for +markdown => html+

Because *markdown* properly does not wrap the html it generates the
header details, nor provide the finishing tags for the html file it
seems logical to provide a *makefile* to complete the generation of
the complete html file or files.

### Example html file produced by this program

```
# makefile for markdown => html
html : index.html page1.html page2.html
# Files named *.o are not object files, they are html stubs, but the
# naming will cause my cleanup scripts to zap them when run.


index.html : indextop.o indexmid.o indexbtm.o
	cat indextop.o indexmid.o indexbtm.o > index.html

indextop.o : indextop.html
	sed 's/filename/index/' indextop.html > indextop.o

indexmid.o : indexmid.md
	markdown indexmid.md > indexmid.o

indexbtm.o : indexbtm.html
	cp indexbtm.html indexbtm.o


page1.html : page1top.o page1mid.o page1btm.o
	cat page1top.o page1mid.o page1btm.o > page1.html

page1top.o : page1top.html
	sed 's/filename/page1/' page1top.html > page1top.o

page1mid.o : page1mid.md
	markdown page1mid.md > page1mid.o

page1btm.o : page1btm.html
	cp page1btm.html page1btm.o


page2.html : page2top.o page2mid.o page2btm.o
	cat page2top.o page2mid.o page2btm.o > page2.html

page2top.o : page2top.html
	sed 's/filename/page2/' page2top.html > page2top.o

page2mid.o : page2mid.md
	markdown page2mid.md > page2mid.o

page2btm.o : page2btm.html
	cp page2btm.html page2btm.o


clean :
	rm *.o
```

### How to produce this *makefile* using this program


```
htmake --new index --add page1 --add page2
```
### Alternatively

```
htmake --new index
htmake --add page1
htmake --add page2
```
