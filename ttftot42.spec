Summary: TrueType font converter
Name: ttftot42
Version: 0.3.1
Release: 4jv
Source: ftp::/ftp.giga.or.at/pub/nih/ttftot42/ttftot42-%{version}.tar.gz
Patch0: %{name}-%{version}.patch0
Patch1: %{name}-%{version}.patch1
URL: http://ftp.giga.or.at/pub/nih/ttftot42
Copyright: Distributable
Group: Utilities/Printing
BuildRoot: /var/tmp/ttftot42

%description
  This is ttftot42, a program to facilitate using TrueType fonts on
PostScript interpreters with TrueType rasterizer.

  You need the FreeType library to compile and use it.

  TrueType fonts cannot be converted to Type 1 fonts without loss of
quality, since conversion introduces rounding errors and loses hinting
information.  Therefore, newer PostScript interpreters (Adobe
PostScript since version 2013, ghostscript since 5.03) include a
TrueType rasterizers.  To use a TrueType font with them, it has to be
embedded in a PostScript font dictionary; this format is called Type 42.

  This program generates such Type 42 fonts from TrueType fonts.  It
can also generate Adobe Font Metrics files (AFM files), including
kerning information.

  Not yet supported are TrueType Collections and the creation of CID
keyed fonts from large TrueType fonts (East Asian or Unicode fonts).

%prep 
%setup
%patch0
%patch1

%build
./configure --prefix=/usr
make all

%install
make install prefix=$RPM_BUILD_ROOT/usr
strip $RPM_BUILD_ROOT/usr/bin/ttftot42

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644, root, root)
%doc README NEWS TODO
%attr (755, root, root) /usr/bin/ttftot42