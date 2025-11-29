package java.lang;

public final class String {

    private final byte[] value;

    public String() {
        this.value = new byte[0];
    }

    public String(byte[] v) {
        this.value = v.clone();
    }

    public native int length();

    public native char charAt(int index);

    public native boolean equals(Object o);

    public native byte[] getBytes();
}
