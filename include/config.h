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
    <div class="container">
        <form>
            <div class="hero">
                <h2>Configuration</h2>
            </div>
            <div class="row">
                <div class="col c12">
                    <h3>Fuel flow</h3>
                </div>
            </div>
            <div class="row">
                <div class="col c2"><label for="mlppIn">Input sensor ml/p:</label></div>
                <div class="col c10"><input type="number" name="mlppIn" value="%FLOW_MLPPIN%" class="smooth" /></div>
            </div>
            <div class="row">
                <div class="col c2"><label for="mlppOut">Output sensor ml/p:</label></div>
                <div class="col c10"><input type="number" name="mlppOut" value="%FLOW_MLPPOUT%" class="smooth" /></div>
            </div>
            <div class="row">
                <div class="col c2"><label for="flowMovAvg">Moving average:</label></div>
                <div class="col c10"> <input type="number" name="flowMovAvg" value="%FLOW_MOVAVG%" class="smooth" />
                </div>
            </div>
            <div class="row">
                <div class="col c12">
                    <h3>Engine RPM</h3>
                </div>
            </div>
            <div class="row">
                <div class="col c2"><label for="mlppIn">Multiplier:</label></div>
                <div class="col c10"><input type="number" name="mlppIn" value="%RPM_MULTIPLIER%" class="smooth" /></div>
            </div>
            <div class="row">
                <div class="col c12">
                    <h3>Coolant temperature</h3>
                </div>
            </div>
            <div class="row">
                <div class="col c2"><label for="mlppIn">Adjustment:</label></div>
                <div class="col c10"><input type="number" name="mlppIn" value="%TEMP_ADJUSTMENT%" class="smooth" />
                </div>
            </div>
            <div class="row">
                <div class="col c12">
                    <h3>Oil preassure</h3>
                </div>
            </div>
            <div class="row">
                <div class="col c2"><label for="mlppIn">Adjustment:</label></div>
                <div class="col c10"><input type="number" name="mlppIn" value="%OIL_ADJUSTMENT%" class="smooth" />
                </div>
            </div>
            <div class="row">
                <div class="col c12"><input type="submit" value="Save" class="btn-sm btn-a"></div>
            </div>
        </form>
    </div>
</body>

</html>
)rawText"