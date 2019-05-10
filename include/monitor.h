R"rawText(
<!DOCTYPE html>
<html>

<head>
    <title>Engine Monitor</title>
</head>

<body>
    <h2>Debug Monitor</h2>
    <p>
        Pulses: <input type="text" id="txtPulses" />
        Flow: <input type="text" id="txtFlow" /><br />
        Elapsed In: <input type="text" id="txtElapsedIn" />
        Elapsed Out: <input type="text" id="txtElapsedOut" /><br />
        Ram: <input type="text" id="txtRam" />
    </p>
    <p>
        <button id='clrButton'>Clear List</button>
    </p>
    <p>
        <ol id='debugList'>
        </ol>
    </p>

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