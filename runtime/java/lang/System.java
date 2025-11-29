package java.lang;

import java.io.PrintStream;

public final class System {

    public static final PrintStream out;

    static {
        out = new PrintStream();
    }

    private System() {
    }
}
