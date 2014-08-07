/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.2

Page {
    Container {
        TextField {
            id: textField
            hintText: "Search term"
        }
        Button {
            text: "Search"
            onClicked: {
                // If the text isn't blank, save it
            	if (textField.text.trim() != "") {
                    _app.searchThis(textField.text)
                    textField.text = ""
                    label.text = "Minimize me to see the new search in the Active Frame"            	    
            	}
            }
        }
        Button {
            text: "Clear all searches"
            onClicked: {
                _app.clearSearches()
                label.text = "All searches cleared"
            }
        }
        Label {
            id: label
            multiline: true
        }
    }
}
