
window.onload = init;

// initialization function to be called on page load
function init(){
    const map = new ol.Map({
        view: new ol.View({
            projection: 'EPSG:4326',    // sets map coordinate system to lon/lat (default is EPSG:3857)
            center: [-97.11325733587586, 32.73282849481263],
            zoom: 16,
            maxZoom: 20,
            minZoom: 4,
            rotation: 0.0,
        }),
        layers: [
            new ol.layer.Tile({
                source: new ol.source.OSM()
            })
        ],
        target: 'js-map'
    })

    // handler for map clicks
    map.on('click', function(e){
        console.log(e.coordinate);
    })

    // styles for map annotations
    const fillStyle = new ol.style.Fill({
        color: [0, 0, 255, 0.25]
    })
    const strokeStyle = new ol.style.Stroke({
        color: [0, 255, 0, 1.0],
        width: 1.2
    })
    const circleStyle = new ol.style.Circle({
        fill: new ol.style.Fill({
            color: [255, 0, 0, 1.0]
        }),
        radius: 5,
        stroke: strokeStyle
    })

    // vector layers
    const geoJSON = new ol.layer.VectorImage({
        source: new ol.source.Vector({
            url: './map.geojson', // geojson map can be created at https://geojson.io/
            format: new ol.format.GeoJSON()
        }),
        visible: true,
        title: 'GeoJSON',
        style: new ol.style.Style({
            fill: fillStyle,
            stroke: strokeStyle,
            image: circleStyle
        })
    })
    map.addLayer(geoJSON);
}