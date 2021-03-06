#include "rtplot.h"
#include <iomanip>
#include <sstream>


//자리수 구함
int getDigit(FLOAT value) {
    if(value == 0.0) { return 0; }
    return (int) ceil(log10(fabs(value)));
     // [0.01,0.1) -> -1 / [0.1,1] -> 0 / (1,10] -> 1 / (10,100] -> 2
}

inline double ceilDigit(double value, int digits) {
    double factor = pow(10.0, -digits); // 1이면 소수점 1자리, -1이면 십의자리
    return ceil(value * factor)/ factor;
}

double roundValue(double value, int digits){ //소수점 자릿수
    if (value == 0.0) // otherwise it will return 'nan' due to the log10() of zero
        return 0.0;
    double factor = pow(10.0, digits - ceil(log10(fabs(value)))); //
    return round(value * factor) / factor;
}

std::string roundString(double value, int digits)
{
    std::stringstream stream;
    if (value == 0.0) {
        stream << std::fixed << std::setprecision(digits-1)<< 0.0;
        return stream.str();
    }

    double n = log10(fabs(value));
    double factor = pow(10.0, digits - ceil(n));
    if(n < 0) {
        stream << std::fixed << std::setprecision(-((int) n)+digits) << (round(value * factor) / factor);
        return stream.str();
    }
    stream << std::fixed << std::setprecision((digits-((int) n)-1) > 0 ? digits-((int) n)-1 : 0) << (round(value * factor) / factor);
    return stream.str();
}

namespace rtplot {


bool DataSet::deleteData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();) {
        if(it->getName() == name) { it = datas.erase(it); return true; }
        it++;
    }
    return false;
}
bool DataSet::deleteData(unsigned int index) {
    if(datas.size() > index ) {
        datas.erase(std::next(datas.begin(), index));
        return true;
    }
    return false;
}

void DataSet::updateData(std::vector<DataStruct>* vec)
{
    for(auto it = vec->begin(); it != vec->end(); ++it ){
        //std::string target = it->getName();
        bool exist = false;


        for(auto itt = datas.begin(); itt != datas.end(); ++itt) {
            if(itt->getName() == it->getName()) {
                exist = true;
                itt->setData(it->getData());
                *(itt->sizePtr()) = it->size();
                break;
            }
        }

        if(exist == false) {
            datas.push_back(*it);
        }



    }
}

DataStruct* DataSet::getData(std::string name) {
    for(auto it = datas.begin(); it != datas.end(); ++it) {
        if(it->getName() == name) { return &(*it); }
    }
    return nullptr;
}
DataStruct* DataSet::getData(unsigned int index) {
    if(datas.size() > index ) {
        return &datas[index];
    }
    return nullptr;
}

void printDataByType(DataStruct& data) {
    if(data.getType() == DataType::SINGLE_INT) {
        auto d = static_cast<const std::vector<INT>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::SINGLE_FLOAT) {
        auto d = static_cast<const std::vector<FLOAT>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::SINGLE_STRING) {
        auto d = static_cast<const std::vector<STRING>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::PAIR_FLOAT_FLOAT) {
        auto d = static_cast<const std::vector<Pair<FLOAT,FLOAT>>*>(data.getData());
        printVector<FLOAT,FLOAT>(*d);
    } else if(data.getType() == DataType::PAIR_STRING_INT) {
        auto d = static_cast<const std::vector<Pair<STRING,INT>>*>(data.getData());
        printVector<STRING,INT>(*d);
    } else if(data.getType() == DataType::PAIR_STRING_FLOAT) {
        auto d = static_cast<const std::vector<Pair<STRING,FLOAT>>*>(data.getData());
        printVector<STRING,FLOAT>(*d);
    }
}

void DataSet::printData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();++it) {
        if(it->getName() == name) {
            std::cout << "DataSet ['" << name << "'] :" << std::endl;
            printDataByType(*it);
        }
    }
}
void DataSet::printData(unsigned int index) {
    if(datas.size() <= index) { return; }
    std::cout << "DataSet ['" << datas[index].getName() << "'] :" << std::endl;
    printDataByType(datas[index]);
}
void DataSet::printAll() {
    for(auto it = datas.begin(); it != datas.end();++it) {
        std::cout << "DataSet ['" << (*it).getName() << "'] :" << std::endl;
        printDataByType(*it);
    }
}



void rtplot::drawData()
{
    double height_per_value = (double) m_plot_view.height/(y_axis.max_value - y_axis.min_value);
    double width_per_value = (double) m_plot_view.width/(x_axis.max_value - x_axis.min_value);
    if(type == DataType::PAIR_FLOAT_FLOAT) {
        for(auto it = m_dataset.getDatas()->begin(); it != m_dataset.getDatas()->end(); ++it) {
            if(it->getType() != type) { continue; }
            const std::vector<FloatFloat>* d = (const std::vector<FloatFloat>*) (it->getData());
            for(auto iit = d->begin(); iit != d->end(); ++iit ){
                size_t tx = (iit->first - x_axis.min_value)*width_per_value;
                size_t ty = (iit->second - y_axis.min_value)*height_per_value;
                Location tl = origin_plot(tx,ty);
                Simd::DrawRectangle(m_plot_view,tl.x-1,tl.y-1,tl.x+1,tl.y+1, Color(0,0,255));
            }
        }
    } else if (type == DataType::SINGLE_FLOAT) {

        std::vector<DataStruct> valid_data;

        for(auto it = m_dataset.getDatas()->begin(); it != m_dataset.getDatas()->end(); ++it) {
            if(it->getType() == DataType::SINGLE_FLOAT||it->getType() == DataType::SINGLE_INT) {
                valid_data.push_back(*it);
            }
        }

        double delta;
        if(principal_axis == 'x') {
            delta = (double) m_plot_view.width/valid_data.size();
        } else {
            delta = (double) m_plot_view.height/valid_data.size();
        }

        for(size_t i = 0; i < valid_data.size(); i++) {
            std::vector<FLOAT>* d = static_cast<std::vector<FLOAT>*>(valid_data[i].getData());

            for(auto it = d->begin(); it != d->end(); ++it) {

                double tx = (*it - x_axis.min_value)*width_per_value;
                double ty = (*it - y_axis.min_value)*height_per_value;
                if(principal_axis == 'x') tx = delta * i + delta/2;
                if(principal_axis == 'y') ty = delta * i + delta/2;
                //Location tl = origin_plot(tx,ty);
                Simd::DrawingLine(m_plot_view,origin_plot(tx,ty+delta/2-1),origin_plot(tx,ty-delta/2+1),Color(0,0,255));
                //Simd::DrawRectangle(m_plot_view,tl.x-1,tl.y-1,tl.x+1,tl.y+1, Color(0,0,255));

            }
        }
    }
}

void rtplot::init() {
    Simd::FillValue(m_view,255);
    Simd::FillValue(m_plot_view,255);
    Simd::FillValue(m_title_view,255);
    Simd::FillValue(view_x_axis,255);
    Simd::FillValue(view_y_axis,255);
    drawPlot();
}


void rtplot::drawPlot() {
    if(title_changed){
        Simd::FillValue(m_title_view,255);
        if(!m_title.empty()) {
            Point tfm =m_title_font.Measure(m_title);
            m_title_font.Draw(m_title_view,m_title, Point((m_title_size.width-tfm.x)/2, (m_title_size.height-tfm.y)/2),color_text);
        }
    }
    //좌표 축
    double height_per_value = (double) m_plot_view.height/(y_axis.max_value - y_axis.min_value);
    double width_per_value = (double) m_plot_view.width/(x_axis.max_value - x_axis.min_value);
    {
        Simd::FillValue(m_plot_view,255);
        Simd::FillValue(view_x_axis,255);
        Simd::FillValue(view_y_axis,255);
        if(y_axis.show) {
            if(type == DataType::PAIR_FLOAT_FLOAT || ( (principal_axis != 'y') && ( (type == DataType::PAIR_STRING_FLOAT) || (type == DataType::PAIR_STRING_INT) || (type == DataType::SINGLE_FLOAT) || (type == DataType::SINGLE_INT))) ) {
                double delta_y_major = (double) m_plot_view.height/(y_axis.max_value-y_axis.min_value)*y_axis.major_interval;
                double delta_y_minor = (double) m_plot_view.height/(y_axis.max_value-y_axis.min_value)*y_axis.minor_interval;
                int dig =(y_axis.major_interval > 1 ? getDigit(y_axis.max_value-y_axis.min_value)+2:  -getDigit(y_axis.minor_interval)+2);

                double approx_min_y = roundValue(y_axis.min_value,dig);
                approx_min_y -= remainder(approx_min_y, y_axis.major_interval);

                for(size_t i = 0; i <= y_axis.minor_tick+1; i++) {
                    int ty = delta_y_minor*i + height_per_value*(approx_min_y-y_axis.min_value);
                    if(ty < 0 || ty > (int) m_plot_size.height+1) { continue; }
                    Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(3, ty),color_axis_minor);
                }
                for(size_t i = 0; i <= y_axis.major_tick+1; i++) {
                    std::string y_value = roundString(approx_min_y+ y_axis.major_interval*i,y_axis.digit);
                    int ty = delta_y_major*i + height_per_value*(approx_min_y-y_axis.min_value);

                    if(ty < 0 || ty > (int) m_plot_size.height+1) { continue; }
                    Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(10, ty),color_axis);
                    if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(1,ty),origin_plot(m_plot_size.width-2,ty),color_grid);
                    Point tp =font.Measure(y_value);
                    font.Draw(view_y_axis,y_value,origin_y_axis(tp.x + 10,ty + tp.y/2),color_text);
                }
                //중심 선
                if(y_axis.min_value < 0 && y_axis.max_value > 0) {
                    size_t ty = (0.0-y_axis.min_value)*height_per_value;
                    Simd::DrawingLine(m_plot_view,origin_plot(1,ty),origin_plot(m_plot_size.width-2,ty),color_axis);
                }
            } else if((type == DataType::SINGLE_FLOAT || type == DataType::SINGLE_INT) && principal_axis == 'y') {
                std::vector<DataStruct> valid_data;
                for(auto it = m_dataset.getDatas()->begin(); it != m_dataset.getDatas()->end(); ++it) {
                    if(it->getType() == DataType::SINGLE_FLOAT||it->getType() == DataType::SINGLE_INT) {
                        valid_data.push_back(*it);
                    }
                }
                if(valid_data.size() > 0) {
                    double delta_y = (double) m_plot_view.height/valid_data.size();
                    for(size_t i = 0; i < valid_data.size(); i++) {
                        std::string str = valid_data[i].getName();
                        int ty = delta_y*i;
                        Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(5, ty),color_axis);
                        if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(1,ty),origin_plot(m_plot_size.width-2,ty),color_grid);
                        Point tp =font.Measure(str);
                        font.Draw(view_y_axis,str,origin_y_axis(tp.x + 10,ty + delta_y/2 + tp.y/2),color_text);
                    }
                    Simd::DrawingLine(view_y_axis,origin_y_axis(0,m_plot_view.height-1),origin_y_axis(5, m_plot_view.height-1),color_axis);
                }
            } else {
                double delta_y = (double) m_plot_view.height/y_axis.data_number;
                for(size_t i = 0; i < y_axis.data_number; i++) {
                    std::string str = "a";
                    int ty = delta_y*i;
                    Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(5, ty),color_axis);
                    Point tp =font.Measure(str);
                    font.Draw(view_y_axis,str,origin_y_axis(tp.x + 10,ty + delta_y/2 + tp.y/2),color_text);
                }

            }
        }

        if(x_axis.show) {
            if(type == DataType::PAIR_FLOAT_FLOAT || ( (principal_axis != 'x') && ( (type == DataType::PAIR_STRING_FLOAT) || (type == DataType::PAIR_STRING_INT) || (type == DataType::SINGLE_FLOAT) || (type == DataType::SINGLE_INT)))) {
                double delta_x_major = (double) m_plot_view.width/(x_axis.max_value-x_axis.min_value)*x_axis.major_interval;
                double delta_x_minor = (double) m_plot_view.width/(x_axis.max_value-x_axis.min_value)*x_axis.minor_interval;

                int dig =(x_axis.major_interval > 1 ? getDigit(x_axis.max_value-x_axis.min_value)+2:  -getDigit(x_axis.max_value-x_axis.min_value)+5); // 간격이 1보다 작은 경우 소수점 자리를 기준으로, 간격이 1보다 클 경우 전체 값 기준으로

                double approx_min_x = roundValue(x_axis.min_value,dig);
                approx_min_x -= remainder(approx_min_x, x_axis.major_interval);

                for(size_t i = 0; i <= x_axis.minor_tick+1; i++) {
                    int tx = delta_x_minor*i + width_per_value*(approx_min_x-x_axis.min_value);
                    if(tx < 0 || tx > (int) m_plot_size.width+1) { continue; }
                    Simd::DrawingLine(view_x_axis,origin_x_axis(tx, 0),origin_x_axis(tx, 3),color_axis_minor);
                }

                for(size_t i = 0; i <= x_axis.major_tick+1; i++) {
                    std::string x_value = roundString(approx_min_x+ x_axis.major_interval*i,x_axis.digit);
                    int tx = delta_x_major*i + width_per_value*(approx_min_x-x_axis.min_value);
                    if(tx < 0 || tx > (int) m_plot_size.width+1) { continue; }
                    Simd::DrawingLine(view_x_axis,origin_x_axis(tx, 0),origin_x_axis(tx, 10),color_axis);
                    if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(tx,1),origin_plot(tx,m_plot_size.height-2),color_grid);
                    Point tp =font.Measure(x_value);
                    font.Draw(view_x_axis,x_value,origin_x_axis(tx - tp.x/2,tp.y + 10),color_text);
                }
                //중심 선
                if(x_axis.min_value < 0 && x_axis.max_value > 0) {
                    size_t tx = (0.0 - x_axis.min_value)*width_per_value;
                    Simd::DrawingLine(m_plot_view,origin_plot(tx,1),origin_plot(tx,m_plot_size.height-2),color_axis);
                }
            } else if((type == DataType::SINGLE_FLOAT || type == DataType::SINGLE_INT) && principal_axis == 'x') {
                std::vector<DataStruct> valid_data;
                for(auto it = m_dataset.getDatas()->begin(); it != m_dataset.getDatas()->end(); ++it) {
                    if(it->getType() == DataType::SINGLE_FLOAT||it->getType() == DataType::SINGLE_INT) {
                        valid_data.push_back(*it);
                    }
                }
                if(valid_data.size() > 0) {
                    double delta_x = (double) m_plot_view.width/valid_data.size();
                    for(size_t i = 0; i < valid_data.size(); i++) {
                        std::string str = valid_data[i].getName();
                        int tx = delta_x*i;
                        Simd::DrawingLine(view_x_axis,origin_x_axis(tx,0),origin_x_axis(tx, 5),color_axis);
                        if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(tx,1),origin_plot(tx,m_plot_size.height-2),color_grid);
                        Point tp =font.Measure(str);
                        font.Draw(view_x_axis,str,origin_x_axis(tx + delta_x/2 - tp.x/2,tp.y + 5),color_text);
                    }
                    Simd::DrawingLine(view_x_axis,origin_x_axis(m_plot_view.width-1,0),origin_x_axis(m_plot_view.width-1, 5),color_axis);
                }
            } else {
                double delta_x = (double) m_plot_view.width/x_axis.data_number;
                for(size_t i = 0; i <  x_axis.data_number; i++) {
                    std::string str = "a";
                    int tx = delta_x*i;
                    Simd::DrawingLine(view_x_axis,origin_x_axis(tx,0),origin_x_axis(tx, 5),color_axis);
                    Point tp =font.Measure(str);
                    font.Draw(view_x_axis,str,origin_x_axis(tx + delta_x/2 - tp.x/2,tp.y + 5),color_text);
                }

            }
        }


    }

    drawData();

    //plot 외부선
    Simd::DrawingLine(m_plot_view,0,0,0,m_plot_size.height,color_axis);
    Simd::DrawingLine(m_plot_view,0,m_plot_size.height-1,m_plot_size.width-1,m_plot_size.height-1,color_axis);
    Simd::DrawingLine(m_plot_view,0,0,m_plot_size.width,0,color_axis);
    Simd::DrawingLine(m_plot_view,m_plot_size.width-1,0,m_plot_size.width-1,m_plot_size.height-1,color_axis);

}

void rtplot::updatePlot() {
    drawPlot();
    if(title_changed) { Simd::Overlay(m_view,m_title_location.x,m_title_location.y,m_title_view); title_changed = false; }
    Simd::Overlay(m_view,x_axis.location.x, x_axis.location.y,view_x_axis);
    Simd::Overlay(m_view,y_axis.location.x, y_axis.location.y,view_y_axis);
    Simd::Overlay(m_view,m_plot_location.x,m_plot_location.y,m_plot_view);
}

void rtplot::movePlot(int x, int y)
{
    if(principal_axis != 'x') {
        double value_per_width = (x_axis.max_value - x_axis.min_value) / m_plot_view.width;
        x_axis.max_value -=  x * value_per_width;
        x_axis.min_value -=  x * value_per_width;
    }
    if(principal_axis != 'y') {
        double value_per_height = (y_axis.max_value - y_axis.min_value) / m_plot_view.height;
        y_axis.max_value +=  y * value_per_height;
        y_axis.min_value += y * value_per_height;
    }
    updatePlot();
}

void rtplot::moveOrigin(Gradation *axis) {
    double middle = (axis->max_value + axis->min_value)/2;
    axis->max_value -= middle;
    axis->min_value -= middle;
    updatePlot();
}

void rtplot::scalePlot(Gradation *axis, FLOAT pivet, int delta)
{
    if(axis->id == principal_axis) { return; }
    double scale = exp((double) delta/200);
    double diff = axis->max_value - axis->min_value;
    double idiff = axis->imax_value - axis->imin_value;
    axis->max_value = pivet + (axis->max_value-pivet)*scale;
    axis->min_value = pivet - (pivet - axis->min_value)*scale;

    if(axis->major_tick > idiff/axis->imajor_interval*2 || axis->major_tick < idiff/axis->imajor_interval*0.5) {
        axis->major_tick = idiff/axis->imajor_interval;
        axis->major_interval = diff/axis->major_tick;
    }
    if(axis->minor_tick > idiff/axis->iminor_interval*2 || axis->minor_tick < idiff/axis->iminor_interval*0.5) {
        axis->minor_tick = idiff/axis->iminor_interval;
        axis->minor_interval = diff/axis->minor_tick;
    }

    axis->major_tick = diff/axis->major_interval;
    axis->minor_tick = diff/axis->minor_interval;

    updatePlot();
}
void rtplot::scaleOrigin(Gradation *axis, FLOAT pivet) {
    double ratio = (axis->imax_value - axis->imin_value) /(axis->max_value - axis->min_value);
    axis->major_interval = axis->imajor_interval;
    axis->minor_interval = axis->iminor_interval;
    axis->major_tick = (axis->imax_value-axis->imin_value)/axis->major_interval;
    axis->minor_tick = (axis->imax_value-axis->imin_value)/axis->minor_interval;
    axis->max_value = (axis->max_value-pivet)*ratio + pivet;
    axis->min_value = (axis->min_value-pivet)*ratio + pivet;

    updatePlot();
}

void rtplot::autoAxis(FLOAT min, FLOAT max)
{
    x_axis.min_value = min;
    x_axis.max_value = max;
    x_axis.major_tick = 10;
    x_axis.major_interval = (x_axis.max_value-x_axis.min_value)/x_axis.major_tick;
    x_axis.minor_tick = 20;
    x_axis.minor_interval = (x_axis.max_value-x_axis.min_value)/x_axis.minor_tick;
}

void rtplot::setDataSet(DataSet dataset, DataType type)
{
    m_dataset = dataset;
    this->type = type;
}
}
