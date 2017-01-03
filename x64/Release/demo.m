close all

bg_folder = '{35517F6A-E932-4E50-A973-F1BBA1104E17}';
src_folder = '{7204DF25-C707-417B-8C2A-23D1C261F622}';
% bg_folder = '{BAD350EB-2602-454C-BF28-CACBA7A92152}';
% src_folder = '{5BF073E0-8902-41F6-A251-6229B317927A}';

bg_list = dir([bg_folder '/*.bmp']);
bg_num = size(bg_list,1);
bg_image_count = zeros(1,64);
bg_image(64).image = [];

for i = 1:bg_num
    bg_cam_id_1 =str2num(bg_list(i).name(10));
    bg_cam_id_2 =str2num(bg_list(i).name(11));
%     
    if(bg_cam_id_2 == '_')
        bg_cam_id =str2num(bg_list(i).name(10));
    else
        bg_cam_id =str2num(bg_list(i).name(10:11));
    end
    
    if(bg_image_count(bg_cam_id) == 0)
        bg_image_count(bg_cam_id) = bg_image_count(bg_cam_id) + 1;

        bg_image(bg_cam_id).bg_file = ['' bg_folder '//' bg_list(i).name];
    end
    
end


%% background remove


im_list = dir([src_folder '/*.bmp']);
im_num = size(im_list,1);

mkdir([src_folder '/mask']);
mkdir([src_folder '/mask/temp'])
% matlabpool open
tic
for i = 1:im_num
    src_cam_id_1 =str2num(im_list(i).name(10));
    src_cam_id_2 =str2num(im_list(i).name(11));
%     
    if(src_cam_id_2 == '_')
        src_cam_id =str2num(im_list(i).name(10));
    else
        src_cam_id =str2num(im_list(i).name(10:11));
    end

    src_file = [ src_folder '//' im_list(i).name];
    
    bg_temp_file = [src_folder '//mask//' im_list(i).name(1:end-3) 'tif'];
    system(['bg_remove.exe ' src_file ' ' bg_image(src_cam_id).bg_file ' ' bg_temp_file]);
    
end
toc