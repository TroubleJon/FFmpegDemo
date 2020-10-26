package com.sean.ffmpegdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = getString(R.string.app_name)
    }

    fun decodeAudio(v: View) {
        val src = "${Environment.getExternalStorageDirectory()}/test.mp3"
        val out = "${Environment.getExternalStorageDirectory()}/out.pcm"
        decodeAudioCpp(src, out)
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private external fun decodeAudioCpp(src: String, out: String)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("seanffmpeg")
            System.loadLibrary("avcodec-57")
            System.loadLibrary("avfilter-6")
            System.loadLibrary("avformat-57")
            System.loadLibrary("avutil-55")
            System.loadLibrary("swresample-2")
            System.loadLibrary("swscale-4")
        }
    }
}
