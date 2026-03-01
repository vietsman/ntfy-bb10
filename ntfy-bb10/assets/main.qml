import bb.cascades 1.4

NavigationPane {
    id: navigationPane
    
    Page {
        titleBar: TitleBar {
            title: "Subscribed topics"
            appearance: TitleBarAppearance.Default
        }
        
        Container {
            layout: DockLayout {}
            
            ListView {
                id: subscriptionList
                dataModel: _subscriptionModel
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            title: ListItemData.topic
                            description: ListItemData.baseUrl
                            imageSource: "asset:///images/topic.png"
                            status: ListItemData.messageCount > 0 ? ListItemData.messageCount : "0" 
                        }
                    }
                ]
                
                onTriggered: {
                    var selectedItem = dataModel.data(indexPath);
                    var detailPage = detailDefinition.createObject();
                    detailPage.topicName = selectedItem.topic;
                    navigationPane.push(detailPage);
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                visible: !_uiController.hasSubscriptions
                
                Label {
                    text: "No subscriptions"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                }
                Label {
                    text: "Tap the plus icon to start."
                    textStyle.base: SystemDefaults.TextStyles.SubtitleText
                }
            }
        }
        
        actions: [
            ActionItem {
                title: "Subscribe"
                imageSource: "asset:///images/ic_add.png"
                ActionBar.placement: ActionBarPlacement.Signature
                onTriggered: {
                    addSheet.open();
                }
            }
        ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: detailDefinition
            source: "TopicDetail.qml"
        },
        Sheet {
            id: addSheet
            content: Page {
                titleBar: TitleBar {
                    title: "Add subscription"
                    dismissAction: ActionItem {
                        title: "Cancel"
                        onTriggered: {
                            addSheet.close();
                        }
                    }
                    acceptAction: ActionItem {
                        title: "Subscribe"
//                        enabled: topicInput.text.length > 0
                        onTriggered: {
                            _uiController.addSubscription(topicInput.text, serverInput.text, customServerToggle.checked);
                            topicInput.text = "";
                            addSheet.close();
                        }
                    }
                }
                Container {
                    topPadding: 40.0
                    leftPadding: 40.0
                    rightPadding: 40.0
                    
                    Label {
                        text: "Topics may not be pfassword-protected, so choose a name that is hard to guess. Once subscribed, you can PUT/POST notifications."
                        multiline: true
                    }
                    
                    TextField {
                        id: topicInput
                        hintText: "Topic name (e.g. alerts)"
                        inputMode: TextFieldInputMode.Text
                    }
                    
                    Container {
                        topPadding: 20.0
                        layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                        
                        Label {
                            text: "Use another server"
                            verticalAlignment: VerticalAlignment.Center
                            // FIXED: Changed StackListLayoutProperties to StackLayoutProperties
                            layoutProperties: StackLayoutProperties { spaceQuota: 1.0 }
                        }
                        
                        CheckBox {
                            id: customServerToggle
                            checked: false
                            verticalAlignment: VerticalAlignment.Center
                        }
                    }
                    
                    TextField {
                        id: serverInput
                        text: "https://ntfy.sh"
                        visible: customServerToggle.checked
                        inputMode: TextFieldInputMode.Url
                    }
                }
            }
        }
    ]
}