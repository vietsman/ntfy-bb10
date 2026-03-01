import bb.cascades 1.4

Page {
    id: topicDetailPage
    property string topicName
    
    titleBar: TitleBar {
        title: topicName
        appearance: TitleBarAppearance.Default
    }
    
    Container {
        layout: DockLayout {}
        
        ListView {
            id: messageList
            // We assign the model directly here
            dataModel: _uiController.getMessageModel(topicName)
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.title
                        description: ListItemData.message
                        status: ListItemData.timestamp
                    }
                }
            ]
        }
        
        // If there are no messages, show a label
        Label {
            text: "No message history"
            visible: messageList.dataModel.size() == 0
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
        }
    }
    
    actions: [
        DeleteActionItem {
            title: "Clear History"
            onTriggered: {
                _uiController.clearMessages(topicName);
                // Refresh the model after clearing
                messageList.dataModel = _uiController.getMessageModel(topicName);
            }
        }
    ]
}