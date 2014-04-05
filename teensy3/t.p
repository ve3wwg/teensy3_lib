--- mk20dx256.ld	2014-04-05 10:51:33.000000000 -0400
+++ mk20dx256_sbrk.ld	2014-04-05 11:11:22.000000000 -0400
@@ -138,5 +138,22 @@
 		__bss_end = .;
 	} > RAM
 
+	/* this is to define the start of the heap, and make sure we have a minimum size */
+	.heap :
+	{
+	    . = ALIGN(4);
+	    _heap_start = .;    /* define a global symbol at heap start */
+	    end = _heap_start;	/* ve3wwg: for sbrk() */
+	    . = . + _minimum_heap_size;
+	} >RAM
+ 
+	/* this just checks there is enough RAM for the stack */
+	.stack :
+	{
+	    . = ALIGN(4);
+	    . = . + _minimum_stack_size;
+	    . = ALIGN(4);
+	} >RAM
+
 	_estack = ORIGIN(RAM) + LENGTH(RAM);
 }
