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

MAINDOC=documentation.pdf
PRESENTDOC=presentation.pdf
APPLYDOC=application.pdf

TEXS=$(TEXDIR)/presentation.tex $(TEXDIR)/application.tex $(TEXDIR)/documentation.tex

TEXRES=$(BDIR)/documentation.pdf $(BDIR)/application.pdf $(BDIR)/presentation.pdf

SHEETS=$(OTHERDIR)/analiza_rynkowa.ods

SHEETRES=$(BDIR)/analiza_rynkowa.pdf

DIAS=$(DIADIR)/groups.dia

DIARES=$(BDIR)/groups.png

PLOTS=$(GRAPHDIR)/export_swiat.plt $(GRAPHDIR)/income_prod.plt $(GRAPHDIR)/polityka_cen.plt $(GRAPHDIR)/porownanie.plt $(GRAPHDIR)/price_weight.plt $(GRAPHDIR)/sale_prod.plt $(GRAPHDIR)/stat_wysz_kanada.plt $(GRAPHDIR)/stat_wysz_polska.plt $(GRAPHDIR)/storage.plt $(GRAPHDIR)/visit_prod.plt

PLOTRES=$(BDIR)/export_swiat.png $(BDIR)/income_prod.png $(BDIR)/polityka_cen.png $(BDIR)/porownanie.png $(BDIR)/price_weight.png $(BDIR)/sale_prod.png $(BDIR)/stat_wysz_kanada.png $(BDIR)/stat_wysz_polska.png $(BDIR)/storage.png $(BDIR)/visit_prod.png

all: clean-log pbuild plots diagrams sheets doxy tex ready

$(TEXRES): $(BDIR)/%.pdf: $(TEXDIR)/%.tex $(TEXDIR)/*.tex  
	TEXINPUTS=$$TEXINPUTS:$(TEXDIR)//:$(BDIR)//:./$(INFODIR)// $(TEXGEN) --build-dir=$(BDIR) $< -o $@ &>> $(LOG)

$(SHEETRES): $(BDIR)/%.pdf: $(OTHERDIR)/%.ods
	$(SHEETGEN) $< --outdir $(BDIR) &>> $(LOG)
	
$(DIARES): $(BDIR)/%.png: $(DIADIR)/%.dia
	$(DIAGEN) $@ $< &>> $(LOG)	

$(PLOTRES): $(BDIR)/%.png: $(GRAPHDIR)/%.plt $(GRAPHDIR)/%.dat
	GNUPLOT_LIB=$(GRAPHDIR) $(PLOTGEN) -e 'set output "$@"' -c $< &>> $(LOG)	

pbuild:
	mkdir -p build

ready: 
	$(CP) $(BDIR)/$(MAINDOC) $(MAINDOC)
	$(CP) $(BDIR)/$(APPLYDOC) $(APPLYDOC)
	$(CP) $(BDIR)/$(PRESENTDOC) $(PRESENTDOC)
	
sheets: $(SHEETRES)

diagrams: $(DIARES)

plots: $(PLOTRES)

tex: $(TEXRES)

doxy: 
	doxygen doxyfile.conf &>> $(LOG)

clean-log:
	$(TRUNC) -s 0 $(LOG)

show-errors:
	$(AWK) '/[Ll][Aa][Tt][Ee][Xx]/{f=1}f' $(LOG) | $(GREP) -nE "[Ee][Rr]{2}[oO][rR]"  || echo -e "\e[32mno errors found"

clean:
	$(REM) $(PLOTRES) $(TEXRES) $(DIARES) $(SHEETRES) $(MAINDOC) $(PRESENTDOC) $(APPLYDOC) $(BDIR)/* *.log *.toc *.aux *.out doxygen/*
