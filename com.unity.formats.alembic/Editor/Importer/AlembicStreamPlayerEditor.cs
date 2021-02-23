using System;
using UnityEngine;
using UnityEditor;
using UnityEngine.Formats.Alembic.Importer;
using UnityEngine.Formats.Alembic.Sdk;

namespace UnityEditor.Formats.Alembic.Importer
{
    [CustomEditor(typeof(AlembicStreamPlayer)), CanEditMultipleObjects]
    class AlembicStreamPlayerEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            serializedObject.Update();
            var streamPlayer = target as AlembicStreamPlayer;
            using (new EditorGUI.DisabledGroupScope((target.hideFlags & HideFlags.NotEditable) != HideFlags.None))
            {
                var streamDescriptorObj = serializedObject.FindProperty("streamDescriptor");
                var startTime = serializedObject.FindProperty("startTime");
                var endTime = serializedObject.FindProperty("endTime");

                var targetStreamDesc = streamPlayer.StreamDescriptor;
                if (streamPlayer.ExternalReference && !serializedObject.isEditingMultipleObjects)
                {
                    var initialFilePath = targetStreamDesc != null ? targetStreamDesc.PathToAbc : "";
                    var filePath = initialFilePath;
                    EditorGUILayout.LabelField(new GUIContent("Alembic File"));
                    using (new EditorGUILayout.HorizontalScope())
                    {
                        filePath = EditorGUILayout.DelayedTextField(filePath);
                        if (GUILayout.Button(new GUIContent("..."), GUILayout.MaxWidth(30)))
                        {
                            var path = EditorUtility.OpenFilePanel("Load Alembic File", "", "abc");
                            if (!string.IsNullOrWhiteSpace(path))
                                filePath = path;
                        }
                    }

                    if (filePath != initialFilePath)
                    {
                        Undo.RegisterFullObjectHierarchyUndo(streamPlayer.gameObject, "Load Alembic File");
                        streamPlayer.LoadFromFile(filePath);
                    }

                    EditorGUILayout.HelpBox(
                        "Alembic File is streamed. Selecting a new file will rebuild the GameObject or Prefab hierarchy.",
                        MessageType.Info);
                    if (streamDescriptorObj.objectReferenceValue == null)
                    {
                        return;
                    }
                }
                else
                {
                    using (new EditorGUI.DisabledGroupScope(true))
                    {
                        EditorGUILayout.ObjectField(streamDescriptorObj);
                    }
                }

                if (streamDescriptorObj.objectReferenceValue == null)
                {
                    EditorGUILayout.HelpBox("The stream descriptor could not be found.", MessageType.Error);
                    return;
                }

                EditorGUILayout.LabelField(new GUIContent("Time Range"));

                var abcStart = targetStreamDesc.mediaStartTime;
                var abcEnd = targetStreamDesc.mediaEndTime;
                var start = streamPlayer.StartTime;
                var end = streamPlayer.EndTime;
                EditorGUI.BeginChangeCheck();
                EditorGUILayout.MinMaxSlider(" ", ref start, ref end, abcStart, abcEnd);
                if (EditorGUI.EndChangeCheck())
                {
                    startTime.doubleValue = start;
                    endTime.doubleValue = end;
                }

                EditorGUILayout.BeginHorizontal();
                EditorGUILayout.PrefixLabel(new GUIContent("seconds"));
                EditorGUI.BeginChangeCheck();
                EditorGUIUtility.labelWidth = 35.0f;
                EditorGUI.showMixedValue = startTime.hasMultipleDifferentValues;
                var newStartTime = EditorGUILayout.FloatField(new GUIContent("from", "Start time"), start,
                    GUILayout.MinWidth(80.0f));
                GUILayout.FlexibleSpace();
                EditorGUIUtility.labelWidth = 20.0f;
                EditorGUI.showMixedValue = endTime.hasMultipleDifferentValues;
                var newEndTime =
                    EditorGUILayout.FloatField(new GUIContent("to", "End time"), end, GUILayout.MinWidth(80.0f));
                EditorGUI.showMixedValue = false;
                if (EditorGUI.EndChangeCheck())
                {
                    startTime.doubleValue = newStartTime;
                    endTime.doubleValue = newEndTime;
                }

                EditorGUILayout.EndHorizontal();
                EditorGUIUtility.labelWidth = 0.0f;

                GUIStyle style = new GUIStyle();
                style.alignment = TextAnchor.LowerRight;
                if (!endTime.hasMultipleDifferentValues && !startTime.hasMultipleDifferentValues)
                {
                    EditorGUILayout.LabelField(new GUIContent((end - start).ToString("0.000") + "s"), style);
                }

                EditorGUILayout.PropertyField(serializedObject.FindProperty("currentTime"), new GUIContent("Time"));
                EditorGUILayout.PropertyField(serializedObject.FindProperty("vertexMotionScale"));
                EditorGUILayout.Space();

                var prefabStatus = PrefabUtility.GetPrefabInstanceStatus(streamPlayer.gameObject);
                if (prefabStatus == PrefabInstanceStatus.NotAPrefab ||
                    prefabStatus == PrefabInstanceStatus.Disconnected)
                {
                    EditorGUILayout.BeginHorizontal();
                    GUILayout.Space(16);
                    if (GUILayout.Button("Recreate Missing Nodes", GUILayout.Width(180)))
                    {
                        streamPlayer.LoadStream(true);
                    }

                    EditorGUILayout.EndHorizontal();
                }
            }

            if (streamPlayer.ExternalReference && !serializedObject.isEditingMultipleObjects)
            {
                DrawStreamSettings(streamPlayer);
            }

            serializedObject.ApplyModifiedProperties();
        }

        static void DrawStreamSettings(AlembicStreamPlayer player)
        {
            using (var check = new EditorGUI.ChangeCheckScope())
            {
                // Draw the stream proeprties
                var pathSettings = "streamSettings.";

                var so = new SerializedObject(player.StreamDescriptor);
                so.Update();
                var settings = so.FindProperty("settings");

                EditorGUILayout.LabelField("Scene", EditorStyles.boldLabel);
                {
                    EditorGUI.indentLevel++;
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("scaleFactor"),
                        new GUIContent("Scale Factor",
                            "How much to scale the models compared to what is in the source file."));
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("swapHandedness"),
                        new GUIContent("Swap Handedness", "Swap X coordinate"));
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("interpolateSamples"),
                        new GUIContent("Interpolate Samples",
                            "Interpolate transforms and vertices (if topology is constant)."));
                    EditorGUILayout.Separator();

                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("importVisibility"),
                        new GUIContent("Import Visibility", "Import visibility animation."));

                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("importCameras"),
                        new GUIContent("Import Cameras", ""));
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("importMeshes"),
                        new GUIContent("Import Meshes", ""));
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("importPoints"),
                        new GUIContent("Import Points", ""));
                    EditorGUILayout.Separator();

                    EditorGUI.indentLevel--;
                }

                EditorGUILayout.LabelField("Geometry", EditorStyles.boldLabel);
                {
                    EditorGUI.indentLevel++;
                    AlembicImporterEditor.DisplayEnumProperty(settings.FindPropertyRelative("normals"),
                        Enum.GetNames(typeof(NormalsMode)));
                    AlembicImporterEditor.DisplayEnumProperty(settings.FindPropertyRelative("tangents"),
                        Enum.GetNames(typeof(TangentsMode)));
                    EditorGUILayout.PropertyField(settings.FindPropertyRelative("flipFaces"));
                    EditorGUI.indentLevel--;
                }
                EditorGUILayout.Separator();

                EditorGUILayout.LabelField("Cameras", EditorStyles.boldLabel);
                {
                    EditorGUI.indentLevel++;
                    AlembicImporterEditor.DisplayEnumProperty(settings.FindPropertyRelative("cameraAspectRatio"),
                        Enum.GetNames(typeof(AspectRatioMode)),
                        new GUIContent("Aspect Ratio", ""));
                    EditorGUI.indentLevel--;
                }
                EditorGUILayout.Separator();

                so.ApplyModifiedProperties();
                if (check.changed)
                {
                    player.Settings = player.StreamDescriptor.Settings;
                }
            }
        }
    }
}
