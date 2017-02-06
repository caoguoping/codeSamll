#include "Helpers.h"
// 
// static Node *_seekNodeByTag(Node *root,int tag,bool head = true)
// {
//     if (!root)
//         return nullptr;
//     
//     if (root->getTag() == tag)
//         return root;
//     
//     const auto& arrayRootChildren = root->getChildren();
//     ssize_t length = arrayRootChildren.size();
//     for (ssize_t i=0;i<length;i++)
//     {
//         Node* child = dynamic_cast<Node*>(arrayRootChildren.at(i));
//         if (child)
//         {
//             Node* res = _seekNodeByTag(child,tag,false);
//             if (res != nullptr)
//                 return res;
//         }
//     }
//     
//     if(head)
//         cocos2d::log("Can't seek node by name:%d",tag);
//     
//     return nullptr;
// }
// 
// Node* Helpers::seekNodeByTag(Node* root, int tag)
// {
//     return _seekNodeByTag(root, tag);
// }
// 
// 
// static Node *_seekNodeByName(Node *root,const std::string &name,bool head = true)
// {
//     if (!root)
//     {
//         cocos2d::log("Null root for seek name:%s",name.c_str());
//         return nullptr;
//     }
//     
//     if (root->getName() == name)
//         return root;
//     
//     const auto& arrayRootChildren = root->getChildren();
//     for (auto& subWidget : arrayRootChildren)
//     {
//         Node* child = dynamic_cast<Node*>(subWidget);
//         if (child)
//         {
//             Node* res = _seekNodeByName(child,name,false);
//             if (res != nullptr)
//                 return res;
//         }
//     }
//     
//     if(head)
//         cocos2d::log("Can't seek node by name:%s",name.c_str());
//     
//     return nullptr;
// }
// 
// Node* Helpers::seekNodeByName(Node* root, const std::string& name)
// {
//     return _seekNodeByName(root, name);
// }
// 
// Node * Helpers::seekNodeByNameFast(Node *root,const std::string &name)
// {
//     if(!root || root->getName() == name)
//         return root;
//     
//     auto vecChildren = &root->getChildren();
//     std::vector<decltype(vecChildren)> vec;
//     vec.push_back(vecChildren);
//     
//     size_t index = 0;
//     do
//     {
//         vecChildren = vec[index];
//         for(auto node : *vecChildren)
//         {
//             if(node->getName() == name)
//             {
//                 return node;
//             }
//             vec.push_back(&node->getChildren());
//         }
//         ++index;
//     } while (index != vec.size());
//     
//     return nullptr;
// }
// 
// 
// Node * Helpers::seekNodeByTagFast(Node *root,int tag)
// {
//     if(!root || root->getTag() == tag)
//         return root;
//     
//     auto vecChildren = &root->getChildren();
//     std::vector<decltype(vecChildren)>vec;
//     vec.push_back(vecChildren);
//     size_t index = 0;
//     do
//     {
//         vecChildren = vec[index];
//         for(auto node : *vecChildren)
//         {
//             if(node->getTag() == tag)
//             {
//                 return node;
//             }
//             vec.push_back(&node->getChildren());
//         }
//         ++index;
//     } while (index != vec.size());
//     
//     return nullptr; 
// }
// 
// 
// void Helpers::pauseNode(Node *node)
// {
//     if(!node)
//         return;
//     
//     node->pause();
//     
//     const auto& arrayRootChildren = node->getChildren();
//     for (auto& subWidget : arrayRootChildren)
//     {
//         Node* child = dynamic_cast<Node*>(subWidget);
//         if (child)
//             pauseNode(child);
//     }
// }
// 
// void Helpers::resumeNode(Node *node)
// {
//     if(!node)
//         return;
//     
//     node->resume();
//     
//     const auto& arrayRootChildren = node->getChildren();
//     for (auto& subWidget : arrayRootChildren)
//     {
//         Node* child = dynamic_cast<Node*>(subWidget);
//         if (child)
//             resumeNode(child);
//     }
// }
// 
// 
// static void _dumpNode(Node *root,int maxDeep,int curDeep = 0)
// {
//     if(!root)
//         return;
//     
//     static std::stringstream ss;
//     ss.str("");
//     
//     if(curDeep)
//     {
//         ss << '+';
//         for(int i = 0; i < curDeep; ++i)
//             ss << "-";
//     }
//     
//     ss << root->getName() << '(' << root->getTag() <<')';
//     cocos2d::log("%s",ss.str().c_str());
//     
//     
//     curDeep++;
//     if(maxDeep < 0 || curDeep <= maxDeep)
//     {
//         const auto& arrayRootChildren = root->getChildren();
//         for (auto& subWidget : arrayRootChildren)
//         {
//             Node* child = dynamic_cast<Node*>(subWidget);
//             if (child)
//                 _dumpNode(child,maxDeep,curDeep);
//         }
//     }
// }
// 
// void Helpers::dumpNode(Node *root,int deep)
// {
//     _dumpNode(root, deep);
// }
// 
// 
// Rect Helpers::getCascadeBoundingBox(Node *node)
// {
//     Rect cbb;
//     Size contentSize = node->getContentSize();
//     
//     // check all childrens bounding box, get maximize box
//     Node* child = nullptr;
//     bool merge = false;
//     for(auto object : node->getChildren())
//     {
//         child = dynamic_cast<Node*>(object);
//         if (!child->isVisible()) continue;
//         
//         const Rect box = getCascadeBoundingBox(child);
//         if (box.size.width <= 0 || box.size.height <= 0) continue;
//         
//         if (!merge)
//         {
//             cbb = box;
//             merge = true;
//         }
//         else
//         {
//             cbb.merge(box);
//         }
//     }
//     
//     // merge content size
//     if (contentSize.width > 0 && contentSize.height > 0)
//     {
//         const Rect box = RectApplyAffineTransform(Rect(0, 0, contentSize.width, contentSize.height), node->getNodeToWorldAffineTransform());
//         if (!merge)
//         {
//             cbb = box;
//         }
//         else
//         {
//             cbb.merge(box);
//         }
//     }
//     
//     return cbb;
// }
