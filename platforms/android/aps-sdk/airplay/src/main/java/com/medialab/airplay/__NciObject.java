package com.medialab.airplay;

import android.util.Log;

/**
 * Represents the NciObject
 */
abstract class NciObject {
  /**
   * The Log tag.
   */
  private static final String TAG = "NciObject";

  /**
   * Loads the JNI module.
   */
  static {
    Log.d(TAG, "static initializer: ");
    System.loadLibrary("aps-jni");
  }

  /**
   * The NCI pointer.
   */
  private long nci_obj_;

  /**
   * Constructs the NciObject.
   */
  public NciObject() { newNci(); }
  /**
   * Creates the NCI resource. The class should implement this method to call
   * the JNI method for constructing the NCI object.
   *
   * @return
   */
  abstract protected void newNci();
  /**
   * Destroys the NCI resources. The class should implement this method to call
   * the JNI method for destroying the NCI object.
   */
  abstract protected void deleteNci();
  /**
   * Finalizes the NCI resource.
   *
   * @throws Throwable
   */
  @Override
  protected synchronized void finalize() throws Throwable {
    super.finalize();
    deleteNci();
  }
}
