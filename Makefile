MGR = billmgr
PLUGIN = pmpaymasterua

CFLAGS += -I/usr/local/mgr5/include/billmgr
CXXFLAGS += -I/usr/local/mgr5/include/billmgr

PKGNAMES = billmanager-plugin-pmpaymasterua
RPM_PKGNAMES ?= $(PKGNAMES)
DEB_PKGNAMES ?= $(PKGNAMES)

WRAPPER += pmpaymasterua paymasteruapayment paymasteruaresult

pmpaymasterua_SOURCES = pmpaymasterua.cpp
pmpaymasterua_LDADD = -lbase -lpaymentmodule
pmpaymasterua_FOLDER = paymethods

paymasteruapayment_SOURCES = paymasteruapayment.cpp
paymasteruapayment_LDADD = -lpaymentcgi
paymasteruapayment_FOLDER = cgi

paymasteruaresult_SOURCES = paymasteruaresult.cpp
paymasteruaresult_LDADD = -lpaymentcgi
paymasteruaresult_FOLDER = cgi

BASE ?= /usr/local/mgr5

# For core <= 5.44, see bug #2604
# XMLLIST += dist/etc/xml/billmgr_mod_pmpaymasterua.xml

include $(BASE)/src/isp.mk
