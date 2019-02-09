#Makefile for documentation generation
#pawel 'prauat' ciejka / pawel.ciejka@robco.pl

PLOTGEN=gnuplot 
TEXGEN=texi2pdf -b
DIAGEN=dia -t png -e 
SHEETGEN=libreoffice --headless --invisible --convert-to pdf
REM=rm -rf
TRUNC=truncate
AWK=awk
GREP=grep
CP=cp

INFODIR=sources
IMGDIR=image
OTHERDIR=tables
DIADIR=diagram
TEXDIR=tex
TABLEDIR=tabele
GRAPHDIR=graph
TPLDIR=template
LOG=build.log
BDIR=build

ACTIVETPL=karta_gwarancyjna_EN.tex karta_gwarancyjna_PL.tex polityka_cookies_PL.tex polityka_cookies_EN.tex regulamin_PL.tex regulamin_EN.tex plan_bp.tex nowy.tex
MAINDOC=nowy.pdf

SOURCES=$(wildcard $(GRAPHDIR)/*.plt) $(wildcard $(DIADIR)/*.dia) $(wildcard $(OTHERDIR)/*.ods) $(ACTIVETPL)
SHEETS=$(filter %.ods,$(SOURCES))
DIAS=$(filter %.dia,$(SOURCES))
PLOTS=$(filter %.plt,$(SOURCES))
TEXS=$(filter %.tex,$(SOURCES))

RESULTTPLS=$(addprefix $(TEXDIR)/,$(ACTIVETPL))
SHEETRES=$(patsubst $(OTHERDIR)/%.ods,$(BDIR)/%.pdf,$(SHEETS))
DIARES=$(patsubst $(DIADIR)/%.dia,$(BDIR)/%.png,$(DIAS))
PLOTRES=$(patsubst $(GRAPHDIR)/%.plt,$(BDIR)/%.png,$(PLOTS))
TEXRES=$(patsubst $(TEXDIR)/%.tex,$(BDIR)/%.pdf,$(RESULTTPLS))

all: clean-log plots diagrams sheets doxy apply tex ready



$(TEXRES): $(BDIR)/%.pdf: $(TEXDIR)/%.tex $(TEXDIR)/*.tex  
	TEXINPUTS=$$TEXINPUTS:$(TEXDIR)//:$(BDIR)//:./$(INFODIR)// $(TEXGEN) --build-dir=$(BDIR) $< -o $@ &>> $(LOG)

$(SHEETRES): $(BDIR)/%.pdf: $(OTHERDIR)/%.ods
	$(SHEETGEN) $< --outdir $(BDIR) &>> $(LOG)
	
$(DIARES): $(BDIR)/%.png: $(DIADIR)/%.dia
	$(DIAGEN) $@ $< &>> $(LOG)	

$(PLOTRES): $(BDIR)/%.png: $(GRAPHDIR)/%.plt $(GRAPHDIR)/%.dat
	GNUPLOT_LIB=$(GRAPHDIR) $(PLOTGEN) -e 'set output "$@"' -c $< &>> $(LOG)	

apply:
	TEXINPUTS=$$TEXINPUTS:$(TEXDIR)//:$(BDIR)//:./$(INFODIR)// $(TEXGEN) --build-dir=$(BDIR) $(TEX)/application.tex -o application.pdf &>> $(LOG)

ready: $(BDIR)/$(MAINDOC) 
	$(CP) $(BDIR)/$(MAINDOC) $(MAINDOC)
	
sheets: $(SHEETRES)

diagrams: $(DIARES)

plots: $(PLOTRES)

tex: $(TEXRES)

doxy: 
	doxygen doxyfile.conf

clean-log:
	$(TRUNC) -s 0 $(LOG)

show-errors:
	$(AWK) '/[Ll][Aa][Tt][Ee][Xx]/{f=1}f' $(LOG) | $(GREP) -nE "[Ee][Rr]{2}[oO][rR]"  || echo -e "\e[32mno errors found"

clean:
	$(REM) $(PLOTRES) $(TEXRES) $(DIARES) $(SHEETRES) $(MAINDOC) $(BDIR)/* *.log *.toc *.aux *.out doxygen/*
