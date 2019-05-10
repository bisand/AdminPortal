R"rawText(
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>Engine Monitor</title>

    <link href="style.css" rel="stylesheet" type="text/css">
</head>

<body>
    <nav class="nav" tabindex="-1" onclick="this.focus()">
        <div class="container">
            <a class="pagename current" href="index.html" style="white-space: nowrap;">Engine Monitor</a>
            <a href="/">Home</a>
            <a href="/docs">Docs</a>
            <a href="/config">Configuration</a>
            <a href="/upgrade">Update Firmware</a>
        </div>
    </nav>
    <button class="btn-close btn btn-sm">×</button>
    <div class="container section">
        <div class="header">
            <h2>Debug monitor</h2>
            <p>
                <label for="txtPulses">Pulses</label>
                <input type="text" id="txtPulses" />
                <label for="txtPulses">Flow</label>
                <input type="text" id="txtFlow" />
                <label for="txtPulses">Elapsed In</label>
                <input type="text" id="txtElapsedIn" />
                <label for="txtPulses">Elapsed Out</label>
                <input type="text" id="txtElapsedOut" />
                <label for="txtPulses">Ram</label>
                <input type="text" id="txtRam" />
            </p>
            <p>
                <button id="clrButton">Clear List</button>
            </p>
        </div>
        <div class="fullcontent">
            <ol id="debugList">
            </ol>
        </div>
    </div>
    <script>
        var eventList = document.querySelector('#debugList');
        var button = document.querySelector('#clrButton');
        button.onclick = function () {
            eventList.innerHTML = "";
        };
        if (!!window.EventSource) {
            var source = new EventSource('/log_events');
            source.addEventListener('log_event', function (e) {
                var newElement = document.createElement("li");
                newElement.textContent = e.data;
                eventList.appendChild(newElement)
            }, false);
            source.addEventListener('pulses', function (e) {
                var newElement = document.querySelector("#txtPulses");
                newElement.value = e.data;
            }, false);
            source.addEventListener('flow', function (e) {
                var newElement = document.querySelector("#txtFlow");
                newElement.value = e.data;
            }, false);
            source.addEventListener('elapsedIn', function (e) {
                var newElement = document.querySelector("#txtElapsedIn");
                newElement.value = e.data;
            }, false);
            source.addEventListener('elapsedOut', function (e) {
                var newElement = document.querySelector("#txtElapsedOut");
                newElement.value = e.data;
            }, false);
            source.addEventListener('ram', function (e) {
                var newElement = document.querySelector("#txtRam");
                newElement.value = e.data;
            }, false);
        }
    </script>
</body>

</html>
)rawText"