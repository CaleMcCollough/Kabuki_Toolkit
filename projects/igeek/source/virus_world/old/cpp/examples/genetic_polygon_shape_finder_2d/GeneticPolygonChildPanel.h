/** Cell Interactive Evolutionary Computation
    @version   1.x
    @file      /.../Cell_IEC/GeneticPolygonChildPanel.h
    @author  Cale McCollough <http://calemccollough.github.io>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                      All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

            http:/www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

namespace Cell_IEC {

class GeneticPolygonChildPanel: public Panel
{
  public:
    
    GeneticPolygonChildPanel (GeneticPolygon fitShape, GeneticPolygon initChild, String thisShapeLabel, int width, int height);
    
    void paintComponent (Graphics g);
    
    GeneticPolygon getChild ();
    
    void setChild (GeneticPolygon newChild);
    
  private:  
    
    GeneticPolygon child, 
        cell;
    
    Label infoLabel;
    
    boolean timerOn;
};
}