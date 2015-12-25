<?php

/**
 * This file contains all of the Batch API callbacks for the custom batch process.
 */

/**
 * Batch node processing (Batch API).
 */
function custom_batch_node_process(&$context) {
  drupal_set_message('Execution has reached custom_batch_node_process...');
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    // Grabs the max number of nodes.
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT nid) FROM {node}')->fetchField();
    // Custom variable for data count.
    $context['sandbox']['count'] = 1;
  }

  // Set the batch size, or amount of nodes to work through at a time.
  $limit = 1;

  // The Batch module passes through this for every grouping of nodes.
  $query = db_select('node', 'n');
  // Sort nodes ascending, and grab next nodes in the batch process.
  $query->fields('n', array('nid'))
    ->condition('nid', $context['sandbox']['current_node'], '>')
    ->orderBy('nid', 'ASC')
    ->range(0, $limit);
  $result = $query->execute();
  foreach ($result as $record) {
    // Here we actually perform our processing on the current node.
    $node = node_load($record->nid, NULL, TRUE);
    $wrapper = entity_metadata_wrapper('node', $node);
    // Arbitrary operation performed on every node.
    $wrapper->title->set('Node #' . $context['sandbox']['count']);
    $wrapper->save();

    // Store some result for post-processing in the finished callback.
    $context['results'][] = check_plain($node->title);

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $node->nid;
    $context['message'] = t('Now processing %node', array('%node' => $node->title));
  }

  $context['sandbox']['count']++;

  // Check if we are finished and give the batch module feedback.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }

}

/**
 * Post-batch operation (Batch API).
 */
function custom_batch_batch_finished() {
  drupal_set_message('Batch process has finished successfully.');
  drupal_goto('/admin/paradox/custom-batch');
}