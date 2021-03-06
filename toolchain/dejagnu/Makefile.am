# Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = dejagnu

# Install all the associated data files
SUBDIRS = doc # testsuite example

# Install the bourne shell script driver in the bindir
bin_SCRIPTS = runtest

# Install the main DejaGnu expect file in the datadir
pkgdata_SCRIPTS = runtest.exp

# Install config.guess too, so we can use it to figure out what type
# of machine we're running on.
libexecdir = $(pkgdatadir)/libexec
libexec_SCRIPTS = config.guess

DATE = `date "+%Y%m%d`

TLCSH = @TCLSH@
PKGDIR = /opt

RUNTESTDEFAULTFLAGS = --srcdir $(srcdir)/testsuite RUNTEST=$(RUNTEST)
RUNTEST = ` \
  if [ -f ${srcdir}/runtest$(EXEEXT) ] ; then \
    echo ${srcdir}/runtest$(EXEEXT); \
  else echo "ERROR: runtest not found" ; exit 1; fi`

CLEANFILES = x.log x.sum site.bak setval.tmp
check-DEJAGNU:
	cd testsuite ; $(MAKE) check
	cd example ; $(MAKE) check

# Since we don't use SUBDIRS, we have to manually clean the
# subdirectories. We don't use SUBDIRS, so the example or test cases
# don't get built by default for a "make all".
clean-local:
	cd doc       ; $(MAKE) clean
	cd testsuite ; $(MAKE) clean
	cd example   ; $(MAKE) clean

# Set ourselves up to build an RPM
tarball: book1.html overview.ps overview.pdf
	-rm -fr dejagnu-${VERSION}
	cp -fpPR $(srcdir) dejagnu-${VERSION}
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	(cd dejagnu-${VERSION} ; $${dir}/Clean.tcl); \
	  cp -R $(srcdir)/doc/html dejagnu-${VERSION}/doc/; \
	  cp $(srcdir)/doc/overview.ps dejagnu-${VERSION}/doc/; \
	  cp $(srcdir)/doc/overview.pdf dejagnu-${VERSION}/doc/; \
	  cp $(srcdir)/doc/overview.rtf dejagnu-${VERSION}/doc/; \
	  tar cvf ../dejagnu-${VERSION}.tar dejagnu-${VERSION}; \
	  gzip -9fv ../dejagnu-${VERSION}.tar

rpmspec: $(srcdir)/packaging/rpm/dejagnu.spec
	cp -f $(srcdir)/packaging/rpm/dejagnu.spec /usr/src/redhat/SPECS/

# Build an RPM package
rpm: tarball rpmspec
	cp -f ../dejagnu-${VERSION}.tar.gz /usr/src/redhat/SOURCES/
	rpm -ba /usr/src/redhat/SPECS/dejagnu.spec

# Build a Debian deb package
deb: book1.html overview.ps overview.pdf
	-rm -fr dejagnu-${VERSION}
	cp -fpr $(srcdir) dejagnu-${VERSION}
	@echo "clean up stuff we don't want in the source file"
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	(cd dejagnu-${VERSION} ; $${dir}/Clean.tcl);
	-mkdir -p doc/html;
	-cp -fr $(srcdir)/packaging/deb dejagnu-${VERSION}/
	-cp -fr doc/html dejagnu-${VERSION}/doc/;
	-cp -fr doc/overview.ps dejagnu-${VERSION}/doc/;
	-cp -fr doc/overview.pdf dejagnu-${VERSION}/doc/;
	-cp -fr doc/overview.rtf dejagnu-${VERSION}/doc/;
	(cd dejagnu-${VERSION}; dpkg-buildpackage)

# Build a Solaris package
solpkg: $(srcdir)/packaging/pkg/prototype
	if test ! -d $(PKGDIR)/dejagnu; then \
	  rm -fr dejagnu-${VERSION}; \
	  mkdir dejagnu-${VERSION}; \
	  cd dejagnu-${VERSION}; \
	  $(srcdir)/configure -v --prefix $(PKGDIR)/dejagnu; \
	  make install ; \
	fi
	cp $(srcdir)/packaging/pkg/prototype /tmp
	cp $(srcdir)/packaging/pkg/pkginfo /tmp
	here=`pwd`; \
	  cd /tmp; \
	  pkgmk -o -r / -d /tmp; \
	  tar cf $$here/dejagnu-${VERSION}.SPARC.2.8.pkg.tar DejaGnu; \
	  gzip -9fv  $$here/dejagnu-${VERSION}.SPARC.2.8.pkg.tar
	rm -fr /tmp/prototype /tmp/pkginfo /tmp/DejaGnu

# Build a HP depot package
hpdepot: # $(srcdir)/packaging/depot/dejagnu.psf
	if test ! -d $(PKGDIR)/dejagnu; then \
	rm -fr dejagnu-${VERSION}; \
	mkdir dejagnu-${VERSION}; \
	cd dejagnu-${VERSION}; \
	$(srcdir)/configure -v --prefix $(PKGDIR)/dejagnu; \
	make install ; \
	fi
	swpackage -s $(srcdir)/packaging/depot/dejagnu.psf -x target_type=directory -d /tmp
	here=`pwd`; \
	cd /tmp; \
	tar cf $$here/dejagnu-${VERSION}.hpux11.depot dejagnu; \
	gzip -9fv  $$here/dejagnu-${VERSION}.hpux11.depot; \
	rm -fr /tmp/dejagnu

book1.html overview.ps overview.pdf install-doc:
	cd doc && $(MAKE) $(FLAGS_TO_PASS) $@

# Automake sucks when it comes to building a distribution, so
# we just copy the source tree onto of what Automake did, and
# then we Clean it. Automake only includes the source files.
# We want all the testsuites, data files, and html versions of the doc.
dist-hook: rpmspec book1.html overview.ps overview.pdf
	-rm -fr $(distdir)
	cp -fpr $(srcdir) $(distdir)
	(cd $(distdir) ; $(TCLSH) $(srcdir)/Clean.tcl)
	-cp -fr doc/html $(distdir)/doc/
	-cp -fr doc/overview.ps $(distdir)/doc/
	-cp -fr doc/overview.pdf $(distdir)/doc/

# Below, host-independent data files that need to get installed.
# We do it this way so we can use globbing.

lib_dest = $(DESTDIR)$(pkgdatadir)
lib_files = $(srcdir)/*.c $(srcdir)/lib/*.exp

baseboards_dest = $(DESTDIR)$(pkgdatadir)/baseboards
baseboards_files = $(srcdir)/baseboards/README $(srcdir)/baseboards/*.exp

include_dest = $(DESTDIR)$(includedir)
config_dest = $(DESTDIR)$(pkgdatadir)/config
config_files = $(srcdir)/config/README $(srcdir)/config/*.exp

install-data-local:
	$(mkinstalldirs) $(lib_dest) $(include_dest)
	for f in $(lib_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(lib_dest); \
	done
	$(mkinstalldirs) $(baseboards_dest)
	for f in $(baseboards_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(baseboards_dest); \
	done
	$(INSTALL_DATA) $(srcdir)/dejagnu.h $(include_dest); \
	$(mkinstalldirs) $(config_dest)
	for f in $(config_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(config_dest); \
	done

uninstall-local:
	for f in $(lib_files); do \
	  test ! -f "$$f" || rm -f $(lib_dest)/`basename "$$f"`; \
	done
	for f in $(config_files); do \
	  test ! -f "$$f" || rm -f $(config_dest)/`basename "$$f"`; \
	done
	for f in $(baseboards_files); do \
	  test ! -f "$$f" || rm -f $(baseboards_dest)/`basename "$$f"`; \
	done

	for f in $(baseboards_files); do \
	  test ! -f "$$f" || rm -f $(baseboards_dest)/`basename "$$f"`; \
	done
	rm -f $(include_dest)/dejagnu.h;

site.exp: Makefile
	@echo 'Making a new site.exp file...'
	@test ! -f site.bak || rm -f site.bak
	@echo '## these variables are automatically generated by make ##' > $@-t
	@echo '# Do not edit here.  If you wish to override these values' >> $-t
	@echo '# edit the last section' >> $@-t
	@echo 'set srcdir $(srcdir)' >> $@-t
	@echo 'set objdir' `pwd` >> $@-t
	@echo '## All variables above are generated by configure. Do Not Edit ##' >> $@-t
	@test ! -f site.exp || sed '1,/^## All variables above are.*##/ d' site.exp >> $@-t
	@test ! -f site.exp || mv site.exp site.bak
	@mv $@-t site.exp
