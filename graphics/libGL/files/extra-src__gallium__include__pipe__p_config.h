--- src/gallium/include/pipe/p_config.h.orig	2012-10-25 04:03:59.000000000 +0900
+++ src/gallium/include/pipe/p_config.h	2013-08-19 15:46:05.000000000 +0900
@@ -106,6 +106,13 @@
 #endif
 #endif
 
+#if defined(__sparc__) || defined(__sparc64__)
+#define PIPE_ARCH_SPARC
+#if defined(__sparc64__)
+#define PIPE_ARCH_SPARC_64
+#endif
+#endif
+
 
 /*
  * Endian detection.
@@ -138,11 +145,20 @@
 # define PIPE_ARCH_BIG_ENDIAN
 #endif
 
+#elif defined(__FreeBSD__)
+#include <machine/endian.h>
+
+#if defined(_LITTLE_ENDIAN)
+# define PIPE_ARCH_LITTLE_ENDIAN
+#elif defined(_BIG_ENDIAN)
+# define PIPE_ARCH_BIG_ENDIAN
+#endif
+
 #else
 
 #if defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)
 #define PIPE_ARCH_LITTLE_ENDIAN
-#elif defined(PIPE_ARCH_PPC) || defined(PIPE_ARCH_PPC_64)
+#elif defined(PIPE_ARCH_PPC) || defined(PIPE_ARCH_PPC_64) || defined(PIPE_ARCH_SPARC) || defined(PIPE_ARCH_SPARC_64)
 #define PIPE_ARCH_BIG_ENDIAN
 #endif
 
