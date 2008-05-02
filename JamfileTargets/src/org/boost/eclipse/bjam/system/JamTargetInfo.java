/* Copyright Frank Birbacher 2008
 * 
 * Use, modification and distribution are subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 */
package org.boost.eclipse.bjam.system;

public class JamTargetInfo {
    /**
     * Name of this target which is shown in the UI.
     */
    private String name;
    /**
     * The commandline string which will be appended to the bjam invocation.
     */
    private String targetName;

    public JamTargetInfo(String name, String targetName) {
        this.name = name;
        this.targetName = targetName;
    }

    public String toString() {
        return name;
    }

    public String getName() {
        return name;
    }

    public String getTargetName() {
        return targetName;
    }
}
