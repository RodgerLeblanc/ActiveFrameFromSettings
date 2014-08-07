import bb.cascades 1.2

Container {
    layout: DockLayout {}
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        rotationZ: -30
        Label {
            objectName: "m_coverLabel"
            textStyle.fontSize: FontSize.Large
            multiline: true
        }        
    }
}
