# $FreeBSD: head/x11/pixman/Makefile 324227 2013-08-04 12:53:54Z az $

PORTNAME=	pixman
PORTVERSION=	0.30.0
CATEGORIES=	x11

MAINTAINER=	x11@FreeBSD.org
COMMENT=	Low-level pixel manipulation library

USES=		perl5
XORG_CAT=	lib
USE_AUTOTOOLS=	libtool
USE_PERL5=	build
USE_GNOME=	ltverhack:9
CONFIGURE_ARGS=	--disable-gtk

OPTIONS_DEFINE=	SIMD

.include <bsd.port.options.mk>

.if ! {PORT_OPTIONS:MSIMD}
CONFIGURE_ARGS+=	--disable-vmx --disable-arm-simd
. if ! ${MACHINE_CPU:Mmmx}
CONFIGURE_ARGS+=	--disable-mmx
. endif
. if ! ${MACHINE_CPU:Msse2}
CONFIGURE_ARGS+=	--disable-sse2
. endif
.endif

.if ${ARCH} == arm || ${ARCH} == armv6
CONFIGURE_ARGS+=        --disable-arm-neon
.endif

.include <bsd.port.mk>
