package com.medialab.airplay;

/**
 *
 */
public class AirPlaySession extends NciObject {
  public static final int MIRROR_SESSION = 0;
  public static final int VIDEO_SESSION = 1;
  public static final int UNKNOWN_SESSION = 0xffffffff;

  /**
   * Creates the native class instance.
   */
  @Override
  protected void newNci() { }

  /**
   * Destroys the native class instance.
   */
  private native void nciDelete();
  @Override
  protected void deleteNci() {
    // Call JIN method
    nciDelete();
  }

  /**
   * @return
   */
  private native long nciGetSessionId();
  public long getSessionId() {
    return nciGetSessionId();
  }

  /**
   * @return
   */
  private native int nciGetSessionType();
  public int getSessionType() {
    return nciGetSessionType();
  }

  /**
   * @param handler
   */
  private native void nciSetMirrorHandler(IAirPlayMirrorHandler handler);
  public void setMirrorHandler(IAirPlayMirrorHandler handler) {
    nciSetMirrorHandler(handler);
  }

  /**
   * @param handler
   */
  private native void nciSetVideoHandler(IAirPlayVideoHandler handler);
  public void setVideoHandler(IAirPlayVideoHandler handler) {
    nciSetVideoHandler(handler);
  }

  /**
   *
   */
  private native void nciDisconnect();
  public void disconnect() {
    nciDisconnect();
  }
}
