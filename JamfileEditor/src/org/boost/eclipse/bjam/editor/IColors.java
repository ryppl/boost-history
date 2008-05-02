/* Copyright Frank Birbacher 2008
 * 
 * Use, modification and distribution are subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 */
package org.boost.eclipse.bjam.editor;

import org.eclipse.swt.graphics.RGB;

public interface IColors {
    RGB SINGLE_COMMENT = new RGB(127, 159, 191);
    RGB COMMANDS = new RGB(63, 127, 95);
    RGB PROPERTIES = new RGB(73, 115, 255);
    RGB MAIN_TARGET_RULE = new RGB(150, 64, 150);
    RGB PUNCTUATION = new RGB(127, 0, 85);
    RGB JAM_BUILTIN = new RGB(191, 127, 95);
    RGB JAM_FLOW = PUNCTUATION;
    RGB JAM_PUNCTUATION = PUNCTUATION;
    RGB VARIABLES = new RGB(64, 150, 128);
    RGB DEFAULT = new RGB(0, 0, 0);
}
