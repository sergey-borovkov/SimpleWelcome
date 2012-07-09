import QtQuick 1.1
import QtWebKit 1.0

WebView {

    focus: true

    onUrlChanged:  {
        console.log("QML")
        authorizer.urlChanged(url)
        if (url.toString().indexOf("http://www.facebook.com/connect/login_success.html") == 0) {
            url = ""
            Qt.quit()
        }
    }
    url: "https://www.facebook.com/dialog/oauth?client_id=148453655273563&redirect_uri=http://www.facebook.com/connect/login_success.html&response_type=token&scope=publish_stream,read_stream"

}